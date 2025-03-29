#pragma once

#include "type_info.h"

// Объявления функций
int freeMatrix(Matrix* matrix);
Matrix* makeMatrix(int string, int elem, const char* type, operations* ops);
Matrix* readMatrix(FILE* prog_fail, operations* ops);
int printMatrix(FILE* prog_finish, Matrix* matrix, operations* ops, char* comment);
int sumMatrix(Matrix* matrix1, Matrix* matrix2, Matrix* result, operations* ops);
int multiplyMatrix(Matrix* matrix1, Matrix* matrix2, Matrix* result, operations* ops);
int skalarMatrix(data_type* scalar, Matrix* matrix, Matrix* result, operations* ops);
int linkombMatrix(Matrix* matrix, Matrix* result, data_type* coef, int index, operations* ops);
int transpose(Matrix* matrix, Matrix* result, operations* ops);
