/********************************************************************************
 * Include Files
 ********************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "util.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

#define SYM_TABLE_SIZE 109
#define BIND_TABLE_SIZE 109

typedef struct bind_*   bind;

struct bind_
{
    SYM_symbol  key;
    void *    value;
    bind      next;       /*< next node in hash bucket */
    SYM_symbol  prevtop;    /*< previous bound symbol */
};

struct SYM_symbol_
{
    const char *name;
    SYM_symbol    next;
};

struct SYM_table_
{
    bind binds[BIND_TABLE_SIZE];
    SYM_symbol top;     /*< newest bound SYM_symbol */
};

/********************************************************************************
 * Private Data
 ********************************************************************************/

static SYM_symbol symtable[SYM_TABLE_SIZE];

/********************************************************************************
 * Private Functions
 ********************************************************************************/

static inline unsigned BKDRhash(const char *s)
{
    unsigned seed = 131, hash = 0;

    while (*s)
        hash = hash * seed + (*s++);

    return hash % SYM_TABLE_SIZE;
}

static inline unsigned ptrhash(SYM_symbol s)
{
    unsigned hash = s;

    return hash % SYM_TABLE_SIZE;
}

static inline void SYM_pop(SYM_table t)
{
    int index;
    bind b;

    index           = ptrhash(t->top);
    b               = t->binds[index];
    t->binds[index] = b->next;
    t->top          = b->prevtop;
}

static void SYM_dump(void)
{
    SYM_symbol s;
    int i;

    printf("--- dump symbol table ---\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (symtable[i])
            printf("  %3d: ", i);

        for (s = symtable[i]; s; s = s->next)
            printf("(%s)", s->name);

        if (symtable[i])
            printf("\n");
    }
}

/********************************************************************************
 * Public Functions
 ********************************************************************************/

SYM_symbol SYM_mk_symbol(const char *name)
{
    unsigned index = BKDRhash(name);
    SYM_symbol head = symtable[index];
    SYM_symbol s;

    for (s = head; s; s = s->next) {
        if (!strcmp(s->name, name))
            return s;
    }

    s = UTL_alloc(sizeof(*s));
    s->name = UTL_strdup(name);
    s->next = head;

    symtable[index] = s;
    return symtable[index];
}

SYM_table SYM_mk_table(void)
{
    SYM_table t = UTL_alloc(sizeof(*t));

    memset(t, 0, sizeof(*t));

    return t;
}

const char *SYM_get_name(SYM_symbol s)
{
    return s->name;
}

void SYM_enter(SYM_table t, SYM_symbol s, void *v)
{
    unsigned index = ptrhash(s);
    bind b;

    if (!t || !s)
        UTL_error(-1, "enter fail");

    b = UTL_alloc(sizeof(*b));
    b->key      = s;
    b->value    = v;
    b->next     = t->binds[index];
    b->prevtop  = t->top;

    t->binds[index] = b;
    t->top = s;
}

void *SYM_look(SYM_table t, SYM_symbol s)
{
    unsigned index = ptrhash(s);
    bind b;

    if (!t || !s)
        UTL_error(-1, "look fail");

    for (b = t->binds[index]; b; b = b->next) {
        if (b->key == s)
            return b->value;
    }

    SYM_dump();
    SYM_show(t);
    return NULL;
}

void SYM_begin(SYM_table t, const char *name)
{
    char buf[64];

    if (!t)
        UTL_error(-1, "begin fail");

    // scope sign start with "!".
    snprintf(buf, sizeof(buf), "!%s", name);

    SYM_enter(t, SYM_mk_symbol(buf), NULL);
}

void SYM_end(SYM_table t)
{
    unsigned index;
    bind b;

    if (!t)
        UTL_error(-1, "end fail");

    // pop till scope sign.
    while (strncmp(SYM_get_name(t->top), "!", 1))
        SYM_pop(t);

    if (t->top)
        SYM_pop(t);
}

void SYM_show(SYM_table t)
{
    bind b;
    int i;

    if (!t)
        UTL_error(-1, "bind table not exists");

    printf("--- show bind table ---\n");
    for (i = 0; i < BIND_TABLE_SIZE; i++) {
        if (t->binds[i])
            printf("  %3d: ", i);

        for (b = t->binds[i]; b; b = b->next)
            printf("(%s)", b->key->name);

        if (t->binds[i])
            printf("\n");
    }
}
