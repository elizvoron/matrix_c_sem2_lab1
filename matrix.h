#ifndef MATRIX_H
#define MATRIX_H

#include "type_info.h"

typedef struct Matrix {
    int rows;
    int cols;
    void*** data;
    const type_info* ti;
} Matrix;

Matrix* matrix_create(int rows, int cols, const type_info* ti);
void matrix_free(Matrix* m);
void matrix_set_element(Matrix* m, int row, int col, void* value);
void* matrix_get_element(const Matrix* m, int row, int col);
void matrix_print(FILE* file, const Matrix* m);
Matrix* matrix_add(const Matrix* a, const Matrix* b);
Matrix* matrix_multiply(const Matrix* a, const Matrix* b);
Matrix* matrix_transpose(const Matrix* m);
void matrix_row_operation(Matrix* m, int target_row, int src_row, const void* scalar);
Matrix* matrix_scalar_multiply(const Matrix* m, const void* scalar);

#endif