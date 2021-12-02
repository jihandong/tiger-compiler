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

typedef struct U_slice_ * U_slice;

struct U_slice_ { void * ptr; U_slice next; };

/****************************************************************************
 * Privates
 ****************************************************************************/

static U_slice slices;

static void Uadd(void* p)
{
    U_slice s = malloc(sizeof(*s));
    if (!s)
        Uerror(-1, "run out of memory");

    s->ptr  = p;
    s->next = slices;
    slices  = s;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void Ufree(void)
{
    int cnt = 0;
    U_slice tmp;

    while(slices) {
        cnt++;
        tmp = slices->next;
        free(slices->ptr);
        free(slices);
        slices = tmp;
    }

    printf("%d slices have been free\n", cnt);
}

void *Ualloc(int size)
{
    void *p = malloc(size);
    if (!p)
        Uerror(-1, "run out of memory");

    Uadd(p);
    return p;
}

char *Ustrdup(const char *s)
{
    char *p = strdup(s);
    if (!p)
        Uerror(-1, "run out of memory");

    Uadd(p);
    return p;
}

void Uerror(int pos, const char *msg)
{
    if (pos > 0)
        printf("Error at %d: %s\n", pos, msg);
    else
        printf("Error: %s\n", msg);

    Ufree();
    exit(1);
}
