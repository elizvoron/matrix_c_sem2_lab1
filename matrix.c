#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Matrix* matrix_create(int rows, int cols, const type_info* ti) {
    Matrix* m = malloc(sizeof(Matrix));
    m->rows = rows;
    m->cols = cols;
    m->ti = ti;
    m->data = malloc(rows * sizeof(void**));
    for (int i = 0; i < rows; i++) {
        m->data[i] = malloc(cols * sizeof(void*));
        for (int j = 0; j < cols; j++) {
            m->data[i][j] = ti->zero();
        }
    }
    return m;
}

void matrix_free(Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->ti->free_element(m->data[i][j]);
        }
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}

Matrix* matrix_add(const Matrix* a, const Matrix* b) {
    if (!a || !b) return NULL;
    if (a->rows != b->rows || a->cols != b->cols || a->ti != b->ti) return NULL;
    Matrix* result = matrix_create(a->rows, a->cols, a->ti);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->cols; j++) {
            void* sum = a->ti->add(a->data[i][j], b->data[i][j]);
            result->data[i][j] = sum;
        }
    }
    return result;
}

Matrix* matrix_multiply(const Matrix* a, const Matrix* b) {
    if (!a || !b) return NULL;
    if (a->cols != b->rows || a->ti != b->ti) return NULL;
    Matrix* result = matrix_create(a->rows, b->cols, a->ti);
    for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < b->cols; j++) {
            void* sum = a->ti->zero();
            for (int k = 0; k < a->cols; k++) {
                void* prod = a->ti->multiply(a->data[i][k], b->data[k][j]);
                void* new_sum = a->ti->add(sum, prod);
                a->ti->free_element(sum);
                a->ti->free_element(prod);
                sum = new_sum;
            }
            result->data[i][j] = sum;
        }
    }
    return result;
}

Matrix* matrix_transpose(const Matrix* m) {
    Matrix* result = matrix_create(m->cols, m->rows, m->ti);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[j][i] = m->ti->copy(m->data[i][j]);
        }
    }
    return result;
}

void matrix_row_operation(Matrix* m, int target_row, int src_row, const void* scalar) {
    for (int j = 0; j < m->cols; j++) {
        void* scaled = m->ti->scalar_multiply(scalar, m->data[src_row][j]);
        void* new_val = m->ti->add(m->data[target_row][j], scaled);
        m->ti->free_element(m->data[target_row][j]);
        m->ti->free_element(scaled);
        m->data[target_row][j] = new_val;
    }
}

Matrix* matrix_scalar_multiply(const Matrix* m, const void* scalar) {
    Matrix* result = matrix_create(m->rows, m->cols, m->ti);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            void* product = m->ti->scalar_multiply(scalar, m->data[i][j]);
            result->data[i][j] = product;
        }
    }
    return result;
}

void matrix_print(FILE* file, const Matrix* m) {
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            m->ti->print(file, m->data[i][j]);
            printf("\t");
        }
        printf("\n");
    }
}

void matrix_set_element(Matrix* m, int row, int col, void* value) {
    assert(row >= 0 && row < m->rows);
    assert(col >= 0 && col < m->cols);
    m->ti->free_element(m->data[row][col]);
    m->data[row][col] = value;
}

void* matrix_get_element(const Matrix* m, int row, int col) {
    assert(row >= 0 && row < m->rows);
    assert(col >= 0 && col < m->cols);
    return m->data[row][col];
}