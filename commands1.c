#include "commands1.h"
#include "matrix.h"
#include <stdlib.h>
#include <string.h>

Matrix* parse_matrix(FILE* file, const type_info* ti) {
    int rows, cols;
    if (fscanf(file, "%d %d", &rows, &cols) != 2) return NULL;
    
    Matrix* m = matrix_create(rows, cols, ti);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char buffer[100];
            if (fscanf(file, "%99s", buffer) != 1) {
                matrix_free(m);
                return NULL;
            }
            void* element = ti->from_str(buffer);
            matrix_set_element(m, i, j, element);
        }
    }
    return m;
}

void write_matrix(FILE* file, const Matrix* m, const char* op_name) {
    if (!m) return;
    fprintf(file, "%s\n", op_name);
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            FILE* tmp = tmpfile();
            m->ti->print(file, m->data[i][j]);
            rewind(tmp);
            char buffer[50];
            fscanf(tmp, "%49s", buffer);
            fclose(tmp);
            fprintf(file, "%s ", buffer);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
}