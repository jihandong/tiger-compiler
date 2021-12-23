/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct UTL_slice_ * UTL_slice;

struct UTL_slice_
{
    void *  ptr;
    UTL_slice next;
};

/****************************************************************************
 * Privates
 ****************************************************************************/

static UTL_slice slices;

static void UTL_add(void* p)
{
    UTL_slice s = malloc(sizeof(*s));
    if (!s)
        UTL_error(0, "run out of memory");

    s->ptr  = p;
    s->next = slices;
    slices  = s;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void UTL_free(void)
{
    int cnt = 0;
    UTL_slice tmp;

    while(slices) {
        cnt++;
        tmp = slices->next;
        free(slices->ptr);
        free(slices);
        slices = tmp;
    }

    printf("%d slices have been free\n", cnt);
}

void *UTL_alloc(int size)
{
    void *p = malloc(size);
    if (!p)
        UTL_error(-1, "run out of memory");

    UTL_add(p);
    return p;
}

char *UTL_strdup(const char *s)
{
    char *p = strdup(s);
    if (!p)
        UTL_error(-1, "run out of memory");

    UTL_add(p);
    return p;
}

void UTL_error(int pos, const char *fmt, ...)
{
    va_list ap;

    if (pos > 0)
        printf("Error at %d:", pos);
    else
        printf("Error:");

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    printf("\n");

    UTL_free();
    exit(1);
}

