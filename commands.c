#include "commands.h"

//команды для обработки пользовательского ввода с помощью функций матриц
void summ(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops) {
    Matrix* matrix2 = readMatrix(prog_fail, ops);
    if (matrix2 == NULL) {
        return;
    }
    Matrix* result = makeMatrix(matrix->string, matrix->elem, matrix->type, ops);
    if (result == NULL) {
        freeMatrix(matrix2);
        return;
    }
    if (sumMatrix(matrix, matrix2, result, ops) != 0) {
        freeMatrix(matrix2);
        freeMatrix(result);
        return;
    }
    char* comment = "sum";
    printMatrix(prog_finish, result, ops, comment);
    freeMatrix(matrix2);
    freeMatrix(result);
}

void mult(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops) {
    Matrix* matrix2 = readMatrix(prog_fail, ops);
    if (matrix2 == NULL) {
        return;
    }
    Matrix* result = makeMatrix(matrix->string, matrix2->elem, matrix->type, ops);
    if (result == NULL) {
        freeMatrix(matrix2);
        return;
    }
    if (multiplyMatrix(matrix, matrix2, result, ops) != 0) {
        freeMatrix(matrix2);
        freeMatrix(result);
        return;
    }
    char* comment = "mult";
    printMatrix(prog_finish, result, ops, comment);
    freeMatrix(matrix2);
    freeMatrix(result);
}

void scal(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops) {
    Matrix* result = makeMatrix(matrix->string, matrix->elem, matrix->type, ops);
    if (result == NULL) {
        return;
    }
    data_type* scalar = (data_type*)malloc(sizeof(data_type));
    if (scalar == NULL) {
        freeMatrix(result);
        return;
    }
    ops->memory(scalar);
    ops->read(prog_fail, scalar);
    if (scalar->data == NULL) {
        free(scalar);
        freeMatrix(result);
        return;
    }
    if (skalarMatrix(scalar, matrix, result, ops) != 0) {
        free(scalar->data);
        free(scalar);
        freeMatrix(result);
        return;
    }
    char* comment = "scal";
    printMatrix(prog_finish, result, ops, comment);
    free(scalar->data);
    free(scalar);
    freeMatrix(result);
}

void linkomb(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops) {
    Matrix* result = makeMatrix(matrix->string, matrix->elem, matrix->type, ops);
    if (result == NULL) {
        return;
    }
    int index;
    if (fscanf(prog_fail, "%d", &index) != 1) {
        freeMatrix(result);
        return;
    }
    data_type* coef = (data_type*)malloc((size_t)(matrix->string) * sizeof(data_type));
    if (coef == NULL) {
        freeMatrix(result);
        return;
    }
    for (int i = 0; i < matrix->string; i++) {
        ops->memory(&coef[i]);
        if (coef[i].data == NULL) {
            for (int j = 0; j < i; j++) {
                free(coef[j].data);
            }
            free(coef);
            freeMatrix(result);
            return;
        }
        ops->read(prog_fail, &coef[i]);
        if (coef[i].data == NULL) {
            for (int j = 0; j <= i; j++) {
                free(coef[j].data);
            }
            free(coef);
            freeMatrix(result);
            return;
        }
    }
    if (linkombMatrix(matrix, result, coef, index, ops) != 0) {
        for (int i = 0; i < matrix->string; i++) {
            free(coef[i].data);
        }
        free(coef);
        freeMatrix(result);
        return;
    }
    char* comment = "linkomb";
    printMatrix(prog_finish, result, ops, comment);
    for (int i = 0; i < matrix->string; i++) {
        free(coef[i].data);
    }
    free(coef);
    freeMatrix(result);
}

void trans(Matrix* matrix, FILE* prog_finish, operations* ops) {
    Matrix* result = makeMatrix(matrix->elem, matrix->string, matrix->type, ops);
    if (result == NULL) {
        return;
    }
    if (transpose(matrix, result, ops) != 0) {
        freeMatrix(result);
        return;
    }
    char* comment = "trans";
    printMatrix(prog_finish, result, ops, comment);
    freeMatrix(result);
}