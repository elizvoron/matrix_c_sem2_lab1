#ifndef TYPE_INFO_H
#define TYPE_INFO_H
#include <stdio.h>

typedef struct type_info type_info;

struct type_info {
    void* (*add)(const void* a, const void* b);
    void* (*multiply)(const void* a, const void* b);
    void* (*scalar_multiply)(const void* scalar, const void* element);
    void* (*zero)();
    void (*free_element)(void* element);
    void (*print)(FILE* file, const void* element);
    void* (*from_str)(const char* str);
    void* (*copy)(const void* element);
};

#endif