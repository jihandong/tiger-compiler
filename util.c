#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *try_malloc(int size)
{
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "ENOMEM\n");
        exit(1);
    }

    return p;
}

char *try_strdup(const char *s)
{
    char *p = strdup(s);
    if (!p) {
        fprintf(stderr, "ENOMEM\n");
        exit(1);
    }

    return p;
}
