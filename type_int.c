#include "type_int.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Реализация операций для целых чисел

void* int_add(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(const int*)a + *(const int*)b;
    return result;
}

void* int_multiply(const void* a, const void* b) {
    int* result = malloc(sizeof(int));
    *result = *(const int*)a * *(const int*)b;
    return result;
}

void* int_scalar_multiply(const void* scalar, const void* element) {
    return int_multiply(scalar, element);
}

void* int_zero() {
    int* zero = malloc(sizeof(int));
    *zero = 0;
    return zero;
}

void int_free_element(void* element) {
    free(element);
}

void int_print(FILE* file, const void* element) {
    fprintf(file, "%d", *(const int*)element);
}

void* int_from_str(const char* str) {
    int* value = malloc(sizeof(int));
    sscanf(str, "%d", value);
    return value;
}

void* int_copy(const void* element) {
    int* copy = malloc(sizeof(int));
    *copy = *(const int*)element;
    return copy;
}

const type_info int_type_info = {
    .add = int_add,
    .multiply = int_multiply,
    .scalar_multiply = int_scalar_multiply,
    .zero = int_zero,
    .free_element = int_free_element,
    .print = int_print,
    .from_str = int_from_str,
    .copy = int_copy
};