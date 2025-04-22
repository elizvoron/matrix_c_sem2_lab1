#include "matrix.h"
#include "type_int.h"
#include "type_float.h"
#include "type_complex.h"
#include "commands1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

#define MAX_LINE_LEN 1024
#define FLOAT_EPS 1e-6

typedef struct {
    char operation[20];
    char type[10];
    char* input_a;
    char* input_b;
    char* expected;
    char* scalar;
    int target_row;
    int src_row;
} TestCase;

bool compare_elements(const type_info* ti, const void* a, const void* b) {
    if (ti == &int_type_info) 
        return *(int*)a == *(int*)b;
    if (ti == &float_type_info) 
        return fabs(*(double*)a - *(double*)b) < FLOAT_EPS;
    if (ti == &complex_type_info) {
        Complex* ca = (Complex*)a;
        Complex* cb = (Complex*)b;
        return fabs(ca->real - cb->real) < FLOAT_EPS * 10 && 
               fabs(ca->imag - cb->imag) < FLOAT_EPS * 10;
    }
    return false;
}

bool matrix_equals(const Matrix* a, const Matrix* b) {
    if (!a || !b || a->rows != b->rows || a->cols != b->cols) return false;
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            void* elem_a = matrix_get_element(a, i, j);
            void* elem_b = matrix_get_element(b, i, j);
            if (!compare_elements(a->ti, elem_a, elem_b)) return false;
        }
    }
    return true;
}

Matrix* parse_matrix_from_str(const char* str, const type_info* ti) {
    char* buffer = strdup(str);
    if (!buffer) return NULL;

    int rows, cols;
    char* token = strtok(buffer, " ");
    if (!token || sscanf(token, "%d", &rows) != 1) {
        free(buffer);
        return NULL;
    }
    token = strtok(NULL, " ");
    if (!token || sscanf(token, "%d", &cols) != 1) {
        free(buffer);
        return NULL;
    }

    FILE* f = fopen("temp_matrix.txt", "w+");
    if (!f) {
        free(buffer);
        return NULL;
    }
    
    fprintf(f, "%d %d\n", rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            token = strtok(NULL, " ");
            if (!token) {
                fclose(f);
                remove("temp_matrix.txt");
                free(buffer);
                return NULL;
            }
            fprintf(f, "%s ", token);
        }
        fprintf(f, "\n");
    }
    
    rewind(f);
    Matrix* m = parse_matrix(f, ti, 0);
    
    fclose(f);
    remove("temp_matrix.txt");
    free(buffer);
    return m;
}

void matrix_to_str(const Matrix* m, char* buffer, size_t buf_size) {
    char temp[50];
    buffer[0] = '\0';
    
    for (int i = 0; i < m->rows; i++) {
        strncat(buffer, "\n[", buf_size - strlen(buffer) - 1);
        for (int j = 0; j < m->cols; j++) {
            void* elem = matrix_get_element(m, i, j);
            if (m->ti == &int_type_info) {
                snprintf(temp, sizeof(temp), "%d", *(int*)elem);
            } else if (m->ti == &float_type_info) {
                snprintf(temp, sizeof(temp), "%.2f", *(double*)elem);
            } else if (m->ti == &complex_type_info) {
                Complex* c = elem;
                snprintf(temp, sizeof(temp), "%.2f+%.2fi", c->real, c->imag);
            }
            strncat(buffer, temp, buf_size - strlen(buffer) - 1);
            if (j < m->cols-1) strncat(buffer, " ", buf_size - strlen(buffer) - 1);
        }
        strncat(buffer, "]", buf_size - strlen(buffer) - 1);
        if (i < m->rows-1) strncat(buffer, " ", buf_size - strlen(buffer) - 1);
    }
}

void run_test(FILE* report, TestCase* tc) {
    const type_info* ti = NULL;
    char input_a_str[10] = "N/A";
    char input_b_str[10] = "N/A";
    char expected_str[10] = "N/A";
    char actual_str[10] = "N/A";
    bool test_result = false;
    char error_msg[50] = "OK";

    if (strcmp(tc->type, "int") == 0) ti = &int_type_info;
    else if (strcmp(tc->type, "float") == 0) ti = &float_type_info;
    else if (strcmp(tc->type, "complex") == 0) ti = &complex_type_info;
    else {
        fprintf(report, "%s|%s|%s|%s|%s|%s|FAIL|Invalid type\n",
                tc->operation, tc->type, input_a_str, input_b_str, expected_str, actual_str);
        return;
    }

    Matrix *A = NULL, *B = NULL, *result = NULL, *expected = NULL;

    A = parse_matrix_from_str(tc->input_a, ti);
    if (A) {
        matrix_to_str(A, input_a_str, sizeof(input_a_str));
    } else {
        strcpy(error_msg, "Parse A failed");
        goto cleanup;
    }

    if (strcmp(tc->operation, "scal") == 0) {
        if (!tc->scalar) {
            strcpy(error_msg, "Missing scalar");
            goto cleanup;
        }
        void* scalar = ti->from_str(tc->scalar);
        result = matrix_scalar_multiply(A, scalar);
        ti->free_element(scalar);
    } 
    else if (strcmp(tc->operation, "trans") == 0) {
        result = matrix_transpose(A);
    }
    else if (strcmp(tc->operation, "linkomb") == 0) {
        if (!tc->scalar) {
            strcpy(error_msg, "Missing scalar");
            goto cleanup;
        }
        void* scalar = ti->from_str(tc->scalar);
        result = matrix_row_operation(A, tc->target_row, tc->src_row, scalar);
        ti->free_element(scalar);
    }
    else {
        B = parse_matrix_from_str(tc->input_b, ti);
        if (B) {
            matrix_to_str(B, input_b_str, sizeof(input_b_str));
        } else {
            strcpy(error_msg, "Parse B failed");
            goto cleanup;
        }
        
        if (strcmp(tc->operation, "add") == 0) {
            result = matrix_add(A, B);
        } 
        else if (strcmp(tc->operation, "mult") == 0) {
            result = matrix_multiply(A, B);
        }
        else {
            strcpy(error_msg, "Unknown operation");
            goto cleanup;
        }
    }

    expected = parse_matrix_from_str(tc->expected, ti);
    if (expected) {
        matrix_to_str(expected, expected_str, sizeof(expected_str));
    } else {
        strcpy(error_msg, "Parse expected failed");
        goto cleanup;
    }

    if (result) {
        matrix_to_str(result, actual_str, sizeof(actual_str));
        test_result = matrix_equals(result, expected);
    } else {
        strcpy(error_msg, "Operation returned NULL");
    }

cleanup:
    fprintf(report, "%s|%s\nInputA:%s\nInputB:%s\nExpected:%s\nActual:%s\n\nResult: %s|%s\n\n",
        tc->operation,
        tc->type,
        input_a_str,
        (strcmp(tc->operation, "scal") == 0 || 
         strcmp(tc->operation, "linkomb") == 0) ? tc->scalar : input_b_str,
        expected_str,
        actual_str,
        test_result ? "PASS" : "FAIL",
        error_msg);
    fflush(report);

    if (A) matrix_free(A);
    if (B) matrix_free(B);
    if (result && result != A) matrix_free(result);
    if (expected) matrix_free(expected);
}

void read_tests(const char* filename, FILE* report) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(report, "ERROR|Failed to open input file\n");
        return;
    }

    fprintf(report, "Operation|Type|InputA|InputB|Expected|Actual|Result|Error\n");
    fflush(report);

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, f)) {
        if (line[0] == '#' || isspace(line[0])) continue;

        TestCase tc = {0};
        char* token = strtok(line, ",");
        strncpy(tc.operation, token, sizeof(tc.operation)-1);

        token = strtok(NULL, ",");
        strncpy(tc.type, token, sizeof(tc.type)-1);

        token = strtok(NULL, ",");
        tc.input_a = token ? strdup(token) : NULL;

        if (strcmp(tc.operation, "scal") == 0) {
            tc.scalar = strdup(strtok(NULL, ","));
            tc.expected = strdup(strtok(NULL, "\n"));
        } 
        else if (strcmp(tc.operation, "linkomb") == 0) {
            char* params = strtok(NULL, ",");
            if (params) {
                sscanf(params, "%d %d", &tc.target_row, &tc.src_row);
                tc.scalar = strdup(params + strlen(params) + 1);
            }
            tc.expected = strdup(strtok(NULL, "\n"));
        } else {
            tc.input_b = strdup(strtok(NULL, ","));
            tc.expected = strdup(strtok(NULL, "\n"));
        }

        run_test(report, &tc);

        free(tc.input_a);
        free(tc.input_b);
        free(tc.expected);
        free(tc.scalar);
    }
    fclose(f);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <input.txt> <report.txt>\n", argv[0]);
        return 1;
    }
    
    FILE* report = fopen(argv[2], "w");
    read_tests(argv[1], report);
    fclose(report);
    return 0;
}