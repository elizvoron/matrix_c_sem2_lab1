#include "commands.h"

int main(void) {
    FILE* prog_fail = fopen("prog_fail.txt", "r");
    if (prog_fail == NULL) {
        perror("main open1");
        return 1;
    }
    FILE* prog_finish = fopen("prog_finish.txt", "w");
    if (prog_finish == NULL) {
        perror("main open2");
        fclose(prog_fail);
        return 1;
    }
    operations* ops = NULL;
    char type[10];
    if (fscanf(prog_fail, "%s", type) != 1) {
        printf("main type");
        fclose(prog_fail);
        fclose(prog_finish);
        return 1;
    }
    if (strcmp(type, "int") == 0) {
        ops = &int_operations;
    } else if (strcmp(type, "float") == 0) {
        ops = &float_operations;
    } else {
        printf("wrong type main");
        fclose(prog_fail);
        fclose(prog_finish);
        return 1;
    }
    Matrix* matrix = readMatrix(prog_fail, ops);
    if (matrix == NULL) {
        printf("main read");
        fclose(prog_fail);
        fclose(prog_finish);
        return 1;
    }
    char comand[10];
    while (1) {
        if (fscanf(prog_fail, "%s ", comand) == EOF) {
            break;
        }
        if (strcmp(comand, "sum") == 0) {
            summ(prog_fail, matrix, prog_finish, ops);
        } else if (strcmp(comand, "mult") == 0) {
            mult(prog_fail, matrix, prog_finish, ops);
        } else if (strcmp(comand, "scal") == 0) {
            scal(prog_fail, matrix, prog_finish, ops);
        } else if (strcmp(comand, "linkomb") == 0) {
            linkomb(prog_fail, matrix, prog_finish, ops);
        } else if (strcmp(comand, "trans") == 0) {
            trans(matrix, prog_finish, ops);
        } else {
            fprintf(prog_finish, "wrong command\n");
            continue;
        }
        fprintf(prog_finish, "\n");
    }
    fclose(prog_fail);
    fclose(prog_finish);
    freeMatrix(matrix);
    printf("i am here");
    return 0;
}