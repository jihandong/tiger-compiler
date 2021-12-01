/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct slice_ * slice;

struct slice_ { void * ptr; slice next; };

/****************************************************************************
 * Privates
 ****************************************************************************/

static slice slices;

static void add_slice(void* p)
{
    slice s = malloc(sizeof(*s));
    if (!s) {
        fprintf(stderr, "ENOMEM\n");
        all_free();
        exit(1);
    }

    s->ptr  = p;
    s->next = slices;
    slices  = s;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void all_free(void)
{
    int cnt = 0;
    slice tmp;

    while(slices) {
        cnt++;
        tmp = slices->next;
        free(slices->ptr);
        free(slices);
        slices = tmp;
    }

    printf("%d slices have been free\n", cnt);
}

void *try_malloc(int size)
{
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "ENOMEM\n");
        all_free();
        exit(1);
    }

    add_slice(p);
    return p;
}

char *try_strdup(const char *s)
{
    char *p = strdup(s);
    if (!p) {
        fprintf(stderr, "ENOMEM\n");
        all_free();
        exit(1);
    }

    add_slice(p);
    return p;
}
