#include "commands1.h"
#include "matrix.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Matrix* parse_matrix(FILE* file, const type_info* ti, int interactive) {
    int rows, cols;
    
    if (fscanf(file, "%d %d", &rows, &cols) != 2) {
        if (interactive) fprintf(stderr, "Error reading matrix dimensions\n");
        return NULL;
    }
    
    int c;
    while ((c = fgetc(file)) != EOF && c != '\n');

    Matrix* m = matrix_create(rows, cols, ti);
    if (!m) return NULL;

    char buffer[1024];
    for (int i = 0; i < rows; i++) {
        if (interactive) printf("Row %d: ", i+1);
        if (fgets(buffer, sizeof(buffer), file) == NULL) {
            matrix_free(m);
            return NULL;
        }

        char* token = strtok(buffer, " \t\n");
        for (int j = 0; j < cols; j++) {
            if (!token) {
                matrix_free(m);
                return NULL;
            }
            void* element = ti->from_str(token);
            matrix_set_element(m, i, j, element);
            token = strtok(NULL, " \t\n");
        }
    }
    return m;
}

void write_matrix(FILE* file, const Matrix* m, const char* op_name) {
    if (!m) return;
    fprintf(file, "%s\n", op_name);
    matrix_print(file, m);
    fprintf(file, "\n");
}