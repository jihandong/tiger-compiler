#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yyparse(void);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        exit(1);
    }

    if (!yyparse()) {
        fprintf(stderr, "Parsing success\n");
    } else {
        fprintf(stderr, "Parsing failed\n");
    }

    fclose(yyin);
    return 0;
}
