#include "matrix.h"
#include "type_int.h"
#include "type_float.h"
#include "type_complex.h"
#include "commands1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    SetConsoleOutputCP(CP_UTF8);
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");
    if (!input || !output) {
        fprintf(stderr, "Error opening files!\n");
        return 1;
    }

    // Определяем тип данных
    const type_info* ti = NULL;
    char type_str[10];
    if (fscanf(input, "%9s", type_str) != 1) {
        fprintf(stderr, "Error reading type!\n");
        fclose(input);
        fclose(output);
        return 1;
    }
    
    if (strcmp(type_str, "int") == 0) ti = &int_type_info;
    else if (strcmp(type_str, "float") == 0) ti = &float_type_info;
    else if (strcmp(type_str, "complex") == 0) ti = &complex_type_info;
    else {
        fprintf(stderr, "Invalid type!\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    // Читаем основную матрицу
    Matrix* matrix = parse_matrix(input, ti);
    if (!matrix) {
        fprintf(stderr, "Error reading main matrix!\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    // Обрабатываем команды
    char command[20];
    while (fscanf(input, "%19s", command) == 1) {
        if (strcmp(command, "sum") == 0) {
            Matrix* b = parse_matrix(input, ti);
            Matrix* res = matrix_add(matrix, b);
            write_matrix(output, res, "sum");
            matrix_free(b);
            matrix_free(res);
        } 
        else if (strcmp(command, "mult") == 0) {
            Matrix* b = parse_matrix(input, ti);
            Matrix* res = matrix_multiply(matrix, b);
            write_matrix(output, res, "mult");
            matrix_free(b);
            matrix_free(res);
        }
        else if (strcmp(command, "scal") == 0) {
            char scalar_str[100];
            fscanf(input, "%99s", scalar_str);
            void* scalar = ti->from_str(scalar_str);
            Matrix* res = matrix_scalar_multiply(matrix, scalar);
            write_matrix(output, res, "scal");
            ti->free_element(scalar);
            matrix_free(res);
        }
        else if (strcmp(command, "linkomb") == 0) {
            int target_row, src_row;
            char scalar_str[100];
            fscanf(input, "%d %d %99s", &target_row, &src_row, scalar_str);
            void* scalar = ti->from_str(scalar_str);
            matrix_row_operation(matrix, target_row, src_row, scalar);
            write_matrix(output, matrix, "linkomb");
            ti->free_element(scalar);
        }
        else if (strcmp(command, "trans") == 0) {
            Matrix* res = matrix_transpose(matrix);
            write_matrix(output, res, "trans");
            matrix_free(res);
        }
    }

    matrix_free(matrix);
    fclose(input);
    fclose(output);
    return 0;
}