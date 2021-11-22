#include <stdio.h>
#include <stdlib.h>

void *try_malloc(int size)
{
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "\nno memory error!\n");
        exit(1);
    }

    return p;
}
