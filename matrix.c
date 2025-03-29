#include "matrix.h"

//Обработка/работа с матрицами
int freeMatrix(Matrix* matrix) {
    if (matrix == NULL) {
        return 0;
    }
    if (matrix->data != NULL) {
        for (int i = 0; i < matrix->string; i++) {
            if (matrix->data[i] != NULL) {
                for (int j = 0; j < matrix->elem; j++) {
                    if (matrix->data[i][j] != NULL) {
                        if (matrix->data[i][j]->data != NULL) {
                            free(matrix->data[i][j]->data);
                        }
                        free(matrix->data[i][j]);
                    }
                }
                free(matrix->data[i]);
            }
        }
        free(matrix->data);
    }
    free(matrix);
    return 0;
}

Matrix* makeMatrix(int string, int elem, const char* type, operations* ops) {
    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }
    matrix->string = string;
    matrix->elem = elem;
    strcpy(matrix->type, type);

    matrix->data = (data_type***)malloc((size_t)(matrix->string) * sizeof(data_type**));
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < matrix->string; i++) {
        matrix->data[i] = (data_type**)malloc((size_t)(matrix->elem) * sizeof(data_type*));
        if (matrix->data[i] == NULL) {
            freeMatrix(matrix);
            return NULL;
        }
        for (int j = 0; j < matrix->elem; j++) {
            matrix->data[i][j] = (data_type*)malloc(sizeof(data_type));
            if (matrix->data[i][j] == NULL) {
                freeMatrix(matrix);
                return NULL;
            }
            ops->memory(matrix->data[i][j]);
        }
    }
    return matrix;
}

Matrix* readMatrix(FILE* prog_fail, operations* ops) {
    int string, elem;
    char type[10];
    if ((fscanf(prog_fail, "%s %d %d", type, &string, &elem)) != 3) {
        return NULL;
    }
    Matrix* matrix = makeMatrix(string, elem, type, ops);
    if (matrix == NULL) {
        return NULL;
    }
    for (int i = 0; i < (matrix->string); i++) {
        for (int j = 0; j < (matrix->elem); j++) {
            if (ops->read(prog_fail, matrix->data[i][j]) == NULL) {
                freeMatrix(matrix);
                return NULL;
            }
        }
    }
    return matrix;
}

int printMatrix(FILE* prog_finish, Matrix* matrix, operations* ops, char* comment) {
    if (matrix == NULL) {
        return 1;
    }
    fprintf(prog_finish, "%s\n", comment);
    for (int i = 0; i < matrix->string; i++) {
        for (int j = 0; j < matrix->elem; j++) {
            if (matrix->data[i][j] == NULL || matrix->data[i][j]->data == NULL) {
                return 1;
            }
            ops->print(prog_finish, matrix->data[i][j]->data);
        }
        fprintf(prog_finish, "\n");
    }
    return 0;
}

int sumMatrix(Matrix* matrix1, Matrix* matrix2, Matrix* result, operations* ops) {
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        return 1;
    }
    if (matrix1->string != matrix2->string || matrix1->elem != matrix2->elem) {
        return 1;
    }
    if (strcmp(matrix1->type, matrix2->type) != 0) {
        return 1;
    }
    for (int i = 0; i < matrix1->string; i++) {
        for (int j = 0; j < matrix1->elem; j++) {
            if (matrix1->data[i][j] == NULL || matrix2->data[i][j] == NULL || result->data[i][j] == NULL) {
                return 1;
            }
            result->data[i][j] = ops->sum(matrix1->data[i][j], matrix2->data[i][j]);
            if (result->data[i][j] == NULL) {
                return 1;
            }
        }
    }
    return 0;
}

int multiplyMatrix(Matrix* matrix1, Matrix* matrix2, Matrix* result, operations* ops) {
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        return 1;
    }
    if (matrix1->elem != matrix2->string) {
        return 1;
    }
    if (strcmp(matrix1->type, matrix2->type) != 0) {
        return 1;
    }
    for (int i = 0; i < result->string; i++) {
        for (int j = 0; j < result->elem; j++) {
            ops->zero(result->data[i][j]);
        }
    }
    for (int i = 0; i < matrix1->string; i++) {
        for (int j = 0; j < matrix2->elem; j++) {
            for (int k = 0; k < matrix1->elem; k++) {
                data_type* product = ops->multiply(matrix1->data[i][k], matrix2->data[k][j]);
                if (product == NULL) {
                    return 1;
                }
                data_type* sum = ops->sum(result->data[i][j], product);
                if (sum == NULL) {
                    free(product->data);
                    free(product);
                    return 1;
                }
                ops->copy(result->data[i][j], sum);
                free(product->data);
                free(product);
                free(sum->data);
                free(sum);
            }
        }
    }
    return 0;
}

int skalarMatrix(data_type* scalar, Matrix* matrix, Matrix* result, operations* ops) {
    if (matrix == NULL || result == NULL || scalar == NULL) {
        return 1;
    }
    if (matrix->data == NULL || result->data == NULL) {
        return 1;
    }
    for (int i = 0; i < matrix->string; i++) {
        for (int j = 0; j < matrix->elem; j++) {
            if (matrix->data[i][j] == NULL || result->data[i][j] == NULL) {
                return 1;
            }
            data_type* product = ops->multiply(matrix->data[i][j], scalar);
            if (product == NULL) {
                return 1;
            }
            ops->copy(result->data[i][j], product);
            free(product->data);
            free(product);
        }
    }
    return 0;
}

int linkombMatrix(Matrix* matrix, Matrix* result, data_type* coef, int index, operations* ops) {
    if (matrix == NULL || result == NULL || coef == NULL) {
        return 1;
    }
    if (index < 0 || index >= matrix->string) {
        return 1;
    }
    for (int i = 0; i < matrix->string; i++) {
        for (int j = 0; j < matrix->elem; j++) {
            if (matrix->data[i][j] == NULL || result->data[i][j] == NULL) {
                return 1;
            }
            ops->copy(result->data[i][j], matrix->data[i][j]);
        }
    }
    for (int j = 0; j < matrix->elem; j++) {
        ops->zero(result->data[index][j]);
        if (result->data[index][j] == NULL) {
            return 1;
        }
    }
    for (int i = 0; i < matrix->string; i++) {
        for (int j = 0; j < matrix->elem; j++) {
            data_type* product = ops->multiply(matrix->data[i][j], &coef[i]);
            if (product == NULL) {
                return 1;
            }
            data_type* sum_result = ops->sum(result->data[index][j], product);
            if (sum_result == NULL) {
                free(product->data);
                free(product);
                return 1;
            }
            ops->copy(result->data[index][j], sum_result);
            free(product->data);
            free(product);
            free(sum_result->data);
            free(sum_result);
        }
    }
    return 0;
}

int transpose(Matrix* matrix, Matrix* result, operations* ops) {
    if (matrix == NULL || result == NULL) {
        return 1;
    }
    for (int i = 0; i < matrix->string; i++) {
        for (int j = 0; j < matrix->elem; j++) {
            ops->copy(result->data[j][i], matrix->data[i][j]);
        }
    }
    return 0;
}