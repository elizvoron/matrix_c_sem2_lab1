#pragma once

#include "matrix.h"

// Объявления функций
void summ(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops);
void mult(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops);
void scal(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops);
void linkomb(FILE* prog_fail, Matrix* matrix, FILE* prog_finish, operations* ops);
void trans(Matrix* matrix, FILE* prog_finish, operations* ops);
