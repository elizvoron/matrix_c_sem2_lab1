#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data_type {
    void* data;
    size_t size;
    struct data_type* (*sum)(struct data_type*, struct data_type*);
    struct data_type* (*multiply)(struct data_type*, struct data_type*);
    void (*print)(FILE*, void* data);
    void (*copy)(struct data_type*, struct data_type*);
    struct data_type* (*read)(FILE*, struct data_type*);
} data_type;

typedef struct {
    int string;
    int elem;
    char type[10];
    data_type*** data;
} Matrix;

typedef struct operations {
    data_type* (*create)(void* value);
    data_type* (*sum)(data_type*, data_type*);
    data_type* (*multiply)(data_type*, data_type*);
    void (*print)(FILE*, void* data);
    void (*copy)(struct data_type*, struct data_type*);
    data_type* (*read)(FILE*, struct data_type*);
    void (*memory)(struct data_type*);
    void (*zero)(struct data_type*);
} operations;

// Объявления функций
data_type* create_int(void* value);
data_type* create_float(void* value);
data_type* int_sum(data_type* a, data_type* b);
data_type* float_sum(data_type* a, data_type* b);
data_type* int_multiply(data_type* a, data_type* b);
data_type* float_multiply(data_type* a, data_type* b);
void int_print(FILE* prog_finish, void* data);
void float_print(FILE* prog_finish, void* data);
void int_copy(struct data_type* a, struct data_type* b);
void float_copy(struct data_type* a, struct data_type* b);
data_type* int_read(FILE* prog_fail, struct data_type* a);
data_type* float_read(FILE* prog_fail, struct data_type* a);
void int_zero(data_type* a);
void float_zero(data_type* a);
void int_memory(data_type* element);
void float_memory(data_type* element);

extern operations int_operations;
extern operations float_operations;
