#ifndef TYPE_COMPLEX_H
#define TYPE_COMPLEX_H

#include "type_info.h"

typedef struct Complex {
    double real;
    double imag;
} Complex;

extern const type_info complex_type_info;

#endif