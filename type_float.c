#include "type_float.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Реализация операций для вещественных чисел

void* float_add(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(const double*)a + *(const double*)b;
    return result;
}

void* float_multiply(const void* a, const void* b) {
    double* result = malloc(sizeof(double));
    *result = *(const double*)a * *(const double*)b;
    return result;
}

void* float_scalar_multiply(const void* scalar, const void* element) {
    return float_multiply(scalar, element);
}

void* float_zero() {
    double* zero = malloc(sizeof(double));
    *zero = 0.0;
    return zero;
}

void float_free_element(void* element) {
    free(element);
}

void float_print(FILE* file, const void* element) {
    fprintf(file, "%.2f", *(const double*)element);
}

void* float_from_str(const char* str) {
    double* value = malloc(sizeof(double));
    sscanf(str, "%lf", value);
    return value;
}

void* float_copy(const void* element) {
    double* copy = malloc(sizeof(double));
    *copy = *(const double*)element;
    return copy;
}

const type_info float_type_info = {
    .add = float_add,
    .multiply = float_multiply,
    .scalar_multiply = float_scalar_multiply,
    .zero = float_zero,
    .free_element = float_free_element,
    .print = float_print,
    .from_str = float_from_str,
    .copy = float_copy
};