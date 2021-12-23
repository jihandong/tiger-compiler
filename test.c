#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "semant.h"
#include "type.h"
#include "util.h"

extern AST_exp AST_root;
int parse(const char *);

int main(int argc, char **argv) {
    const char *sep = "-----------------------------------------------------";
    FILE* fp;
    char ch;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }

    printf("\n%s\nStep 1. parsing:\n", sep);
    if (parse(argv[1]) != 0)
        UTL_error(-1, "parse fail");

    printf("\n%s\nStep 2. contrast:\n", sep);
    fp = fopen(argv[1], "r");
    if (!fp)
        UTL_error(-1, "open fail");
    while((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);

    printf("\n%s\nStep 3. display ast:\n", sep);
    AST_print(stdout, AST_root);

    printf("\n%s\nStep 4. semantic check:\n", sep);
    SMT_trans(AST_root);

    printf("\n%s\nsuccess\n", sep);
    UTL_free();
    return 0;
}
