#ifndef COMMANDS1_H
#define COMMANDS1_H

#include "matrix.h"
#include <stdio.h>

Matrix* parse_matrix(FILE* file, const type_info* ti, int interactive);
void write_matrix(FILE* file, const Matrix* m, const char* op_name);

#endif