#ifndef COMMANDS_H
#define COMMANDS_H

#include "matrix.h"
#include <stdio.h>

// Объявления функций для обработки команд
Matrix* parse_matrix(FILE* file, const type_info* ti);
void write_matrix(FILE* file, const Matrix* m, const char* op_name);

#endif