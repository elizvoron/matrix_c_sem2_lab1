#include "type_info.h"

data_type* create_int(void* value) {
    int value1 = *(int*)value;
    data_type* res = (data_type*)malloc(sizeof(data_type));
    if (!res) return NULL;

    res->data = malloc(sizeof(int));
    if (!res->data) {
        free(res);
        return NULL;
    }

    *(int*)res->data = value1;
    res->size = sizeof(int);
    res->sum = int_sum;
    res->multiply = int_multiply;
    res->print = int_print;
    res->copy = int_copy;
    res->read = int_read;
    return res;
}

data_type* create_float(void* value) {
    float value1 = *(float*)value;
    data_type* res = (data_type*)malloc(sizeof(data_type));
    if (!res) return NULL;

    res->data = malloc(sizeof(float));
    if (!res->data) {
        free(res);
        return NULL;
    }

    *(float*)res->data = value1;
    res->size = sizeof(float);
    res->sum = float_sum;
    res->multiply = float_multiply;
    res->print = float_print;
    res->copy = float_copy;
    res->read = float_read;
    return res;
}

data_type* int_sum(data_type* a, data_type* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    int sum = *(int*)a->data + *(int*)b->data;
    return create_int(&sum);
}

data_type* float_sum(data_type* a, data_type* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    float sum = *(float*)a->data + *(float*)b->data;
    return create_float(&sum);
}

data_type* int_multiply(data_type* a, data_type* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    int product = *(int*)a->data * *(int*)b->data;
    return create_int(&product);
}

data_type* float_multiply(data_type* a, data_type* b) {
    if (!a || !b || !a->data || !b->data) return NULL;
    float product = *(float*)a->data * *(float*)b->data;
    return create_float(&product);
}

void int_print(FILE* prog_finish, void* data) {
    if (prog_finish == NULL || data == NULL) {
        printf("NULL");
        return;
    }
    fprintf(prog_finish, "%d ", *(int*)data);
}

void float_print(FILE* prog_finish, void* data) {
    if (prog_finish == NULL || data == NULL) {
        printf("NULL");
        return;
    }
    fprintf(prog_finish, "%f ", *(float*)data);
}

void int_copy(struct data_type* a, struct data_type* b) {
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        printf("NULL");
        return;
    }
    *(int*)a->data = *(int*)b->data;
}

void float_copy(struct data_type* a, struct data_type* b) {
    if (a == NULL || b == NULL || a->data == NULL || b->data == NULL) {
        printf("NULL");
        return;
    }
    *(float*)a->data = *(float*)b->data;
}

data_type* int_read(FILE* prog_fail, struct data_type* a) {
    if (a == NULL || a->data == NULL || prog_fail == NULL) {
        printf("int read");
        return NULL;
    }
    if (fscanf(prog_fail, "%d", (int*)a->data) != 1) {
        printf("int read2");
        return NULL;
    }
    return a;
}

data_type* float_read(FILE* prog_fail, struct data_type* a) {
    if (a == NULL || a->data == NULL || prog_fail == NULL) {
        printf("float read");
        return NULL;
    }
    if (fscanf(prog_fail, "%f", (float*)a->data) != 1) {
        printf("float read2");
        return NULL;
    }
    return a;
}

void int_zero(data_type* a) {
    if (a == NULL || a->data == NULL) {
        return;
    }
    *(int*)a->data = 0;
}

void float_zero(data_type* a) {
    if (a == NULL || a->data == NULL) {
        return;
    }
    *(float*)a->data = 0.0;
}

void int_memory(data_type* element) {
    if (element == NULL) {
        printf("memory int");
        return;
    }
    element->data = malloc(sizeof(int));
    if (element->data == NULL) {
        printf("memory");
        return;
    }
}

void float_memory(data_type* element) {
    if (element == NULL) {
        printf("memory float");
        return;
    }
    element->data = malloc(sizeof(float));
    if (element->data == NULL) {
        printf("memory f");
        return;
    }
}

operations int_operations = {
    .create = create_int,
    .sum = int_sum,
    .multiply = int_multiply,
    .print = int_print,
    .copy = int_copy,
    .read = int_read,
    .memory = int_memory,
    .zero = int_zero
};

operations float_operations = {
    .create = create_float,
    .sum = float_sum,
    .multiply = float_multiply,
    .print = float_print,
    .copy = float_copy,
    .read = float_read,
    .memory = float_memory,
    .zero = float_zero
};