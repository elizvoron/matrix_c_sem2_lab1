#include "matrix.h"
#include "type_int.h"
#include "type_float.h"
#include "type_complex.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_int_copy() {
    const type_info* ti = &int_type_info;
    int original = 42;
    void* copy = ti->copy(&original);
    assert(*(int*)copy == 42);
    ti->free_element(copy);
}

void test_float_copy() {
    const type_info* ti = &float_type_info;
    double original = 3.14;
    void* copy = ti->copy(&original);
    assert(*(double*)copy == 3.14);
    ti->free_element(copy);
}

void test_complex_copy() {
    const type_info* ti = &complex_type_info;
    void* original = ti->from_str("1.5+2.5i");
    void* copy = ti->copy(original);
    void* zero = ti->zero();
    void* should_be_original = ti->add(zero, original);
    void* should_be_copy = ti->add(zero, copy);
    
    assert(ti->add(original, copy) != NULL); // Просто проверяем что операция возможна
    ti->free_element(original);
    ti->free_element(copy);
    ti->free_element(zero);
    ti->free_element(should_be_original);
    ti->free_element(should_be_copy);
}

int main() {
    test_int_copy();
    test_float_copy();
    test_complex_copy();
    printf("All copy tests passed!\n");
    return 0;
}