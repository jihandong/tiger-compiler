#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "util.h"

extern A_exp A_root;
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
    if (parse(argv[1]) != 0) {
        fprintf(stderr, "parse error\n");
        exit(1);
    }

    printf("\n%s\nStep 2. contrast:\n", sep);
    fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "open error?\n");
        exit(1);
    }
    while((ch = fgetc(fp)) != EOF)
        putchar(ch);
    fclose(fp);

    printf("\n%s\nStep 3. ast:\n", sep);
    A_pr_print(stdout, A_root);

    printf("\n%s\nsuccess\n", sep);
    U_free();
    return 0;
}
