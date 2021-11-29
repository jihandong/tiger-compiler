#include <stdio.h>
#include "ast.h"
#include "show.h"

extern ast_exp parse(const char *);

int main(int argc, char **argv) {
    ast_exp root;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }

    root = parse(argv[1]);
    if (!root) {
        fprintf(stderr, "parse error\n");
        exit(1);
    }

    ast_pr(stdout, root);

    return 0;
}
