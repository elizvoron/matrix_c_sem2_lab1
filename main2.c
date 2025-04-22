#include "matrix.h"
#include "type_int.h"
#include "type_float.h"
#include "type_complex.h"
#include "commands1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void print_menu() {
    printf("Выберите режим ввода:\n");
    printf("1. Использовать файлы (ввод и вывод)\n");
    printf("2. Интерактивный ввод через консоль\n");
    printf("Введите номер режима: ");
}

int get_choice() {
    int choice;
    while (1) {
        if (scanf("%d", &choice) == 1 && (choice == 1 || choice == 2)) {
            while (getchar() != '\n');
            break;
        }
        printf("Ошибка: введите 1 или 2: ");
        while (getchar() != '\n');
    }
    return choice;
}

void get_file_paths(char* input_path, char* output_path) {
    printf("Введите путь к входному файлу: ");
    scanf("%99s", input_path);
    printf("Введите путь к выходному файлу: ");
    scanf("%99s", output_path);
}

void print_result_to_console(const Matrix* m, const char* op_name) {
    printf("\nРезультат операции '%s':\n", op_name);
    matrix_print(stdout, m);
    printf("\n");
}

int main(int argc, char* argv[]) {
    FILE* input = NULL;
    FILE* output = NULL;
    int interactive = 0;

    if (argc == 3) {
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    } else if (argc == 1) {
        SetConsoleOutputCP(CP_UTF8);
        print_menu();
        int choice = get_choice();
        if (choice == 1) {
            char input_path[100], output_path[100];
            get_file_paths(input_path, output_path);
            input = fopen(input_path, "r");
            output = fopen(output_path, "w");
        } else {
            interactive = 1;
            output = fopen("output.txt", "w");
            input = stdin;
            printf("Режим интерактивного ввода. Вводите данные.\n");
        }
    } else {
        fprintf(stderr, "Usage: %s [<input_file> <output_file>]\n", argv[0]);
        return 1;
    }

    if (!input || !output) {
        fprintf(stderr, "Ошибка открытия файлов!\n");
        return 1;
    }

    const type_info* ti = NULL;
    char type_str[10];
    if (interactive) printf("Введите тип данных (int, float, complex): ");
    if (fscanf(input, "%9s", type_str) != 1) {
        fprintf(stderr, "Ошибка чтения типа!\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    if (strcmp(type_str, "int") == 0) ti = &int_type_info;
    else if (strcmp(type_str, "float") == 0) ti = &float_type_info;
    else if (strcmp(type_str, "complex") == 0) ti = &complex_type_info;
    else {
        fprintf(stderr, "Неверный тип!\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    Matrix* matrix = NULL;
    if (interactive) printf("Введите основную матрицу (строки через Enter):\n");
    matrix = parse_matrix(input, ti, interactive);
    if (!matrix) {
        fprintf(stderr, "Ошибка чтения матрицы!\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    char command[20];
    while (1) {
        if (interactive) printf("\nВведите команду (sum, mult, scal, linkomb, trans) или 'exit': ");
        if (fscanf(input, "%19s", command) != 1) break;
        if (strcmp(command, "exit") == 0) break;

        Matrix* res = NULL;
        if (strcmp(command, "sum") == 0) {
            if (interactive) printf("Введите вторую матрицу для сложения:\n");
            Matrix* b = parse_matrix(input, ti, interactive);
            if (!b) {
                fprintf(stderr, "Ошибка чтения второй матрицы!\n");
                continue;
            }
            res = matrix_add(matrix, b);
            matrix_free(b);
        } 
        else if (strcmp(command, "mult") == 0) {
            if (interactive) printf("Введите вторую матрицу для умножения:\n");
            Matrix* b = parse_matrix(input, ti, interactive);
            res = matrix_multiply(matrix, b);
            matrix_free(b);
        }
        else if (strcmp(command, "scal") == 0) {
            if (interactive) printf("Введите скаляр: ");
            char scalar_str[100];
            fscanf(input, "%99s", scalar_str);
            void* scalar = ti->from_str(scalar_str);
            res = matrix_scalar_multiply(matrix, scalar);
            ti->free_element(scalar);
        }
        else if (strcmp(command, "linkomb") == 0) {
            int target_row, src_row;
            char scalar_str[100];
            if (interactive) printf("Введите целевую строку, исходную строку и скаляр: ");
            fscanf(input, "%d %d %99s", &target_row, &src_row, scalar_str);
            void* scalar = ti->from_str(scalar_str);
            
            res = matrix_row_operation(matrix, target_row-1, src_row-1, scalar);
            write_matrix(output, res, "linkomb");
            if (interactive) print_result_to_console(res, "linkomb");
            matrix_free(res); 
            ti->free_element(scalar);
        }
        else if (strcmp(command, "trans") == 0) {
            res = matrix_transpose(matrix);
        }
        else {
            fprintf(stderr, "Неизвестная команда!\n");
            continue;
        }

        if (res) {
            write_matrix(output, res, command);
            fflush(output);
            if (interactive) print_result_to_console(res, command);
            if (res != matrix) matrix_free(res);
        }
    }

    matrix_free(matrix);
    fclose(input);
    fclose(output);
    return 0;
}