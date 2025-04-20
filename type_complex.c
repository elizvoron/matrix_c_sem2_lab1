#include "type_complex.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void* complex_add(const void* a, const void* b) {
    const Complex* ca = a;
    const Complex* cb = b;
    Complex* result = malloc(sizeof(Complex));
    result->real = ca->real + cb->real;
    result->imag = ca->imag + cb->imag;
    return result;
}

static void* complex_multiply(const void* a, const void* b) {
    const Complex* ca = a;
    const Complex* cb = b;
    Complex* result = malloc(sizeof(Complex));
    result->real = ca->real * cb->real - ca->imag * cb->imag;
    result->imag = ca->real * cb->imag + ca->imag * cb->real;
    return result;
}

static void* complex_scalar_multiply(const void* scalar, const void* element) {
    return complex_multiply(scalar, element);
}

static void* complex_zero() {
    Complex* zero = malloc(sizeof(Complex));
    zero->real = 0.0;
    zero->imag = 0.0;
    return zero;
}

static void complex_free_element(void* element) {
    free(element);
}

static void complex_print(FILE* file, const void* element) {
    const Complex* c = element;
    fprintf(file, "%.2f + %.2fi", c->real, c->imag);
}

static void* complex_from_str(const char* str) {
    Complex* c = malloc(sizeof(Complex));
    if(sscanf(str, "%lf+%lfi", &c->real, &c->imag) != 2) {
        free(c);
        return NULL;
    }
    return c;
}

static void* complex_copy(const void* element) {
    const Complex* src = element;
    Complex* copy = malloc(sizeof(Complex));
    copy->real = src->real;
    copy->imag = src->imag;
    return copy;
}

const type_info complex_type_info = {
    .add = complex_add,
    .multiply = complex_multiply,
    .scalar_multiply = complex_scalar_multiply,
    .zero = complex_zero,
    .free_element = complex_free_element,
    .print = complex_print,
    .from_str = complex_from_str,
    .copy = complex_copy
};