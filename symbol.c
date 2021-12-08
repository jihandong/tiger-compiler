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
    S_symbol  key;
    void *    value;
    bind      next;       /*< next node in hash bucket */
    S_symbol  prevtop;    /*< previous bound symbol */
};

struct S_symbol_
{
    const char *name;
    S_symbol    next;
};

struct S_table_
{
    bind binds[BIND_TABLE_SIZE];
    S_symbol top;     /*< newest bound S_symbol */
};

/********************************************************************************
 * Private Data
 ********************************************************************************/

static S_symbol symtable[SYM_TABLE_SIZE];

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

static inline unsigned ptrhash(S_symbol s)
{
    unsigned hash = s;

    return hash % SYM_TABLE_SIZE;
}

static inline void S_pop(S_table t)
{
    int index;
    bind b;

    index           = ptrhash(t->top);
    b               = t->binds[index];
    t->binds[index] = b->next;
    t->top          = b->prevtop;
}

static void S_dump(S_table t)
{
    S_symbol s;
    bind b;
    int i;

    if (!t)
        U_error(-1, "dump fail");

    printf("-- symbol table --\n");
    for(i = 0; i < SYM_TABLE_SIZE; i++) {
        if (symtable[i])
            printf("  ");

        for (s = symtable[i]; s; s = s->next)
            printf("(%s)", s->name);

        if (symtable[i])
            printf("\n");
    }

    printf("-- bind table --\n");
    for (i = 0; i < BIND_TABLE_SIZE; i++) {
        if (t->binds[i])
            printf("  ");

        for (b = t->binds[i]; b; b = b->next)
            printf("(%s)", b->key->name);

        if (t->binds[i])
            printf("\n");
    }
}

/********************************************************************************
 * Public Functions
 ********************************************************************************/

S_symbol S_mk_symbol(const char *name)
{
    unsigned index = BKDRhash(name);
    S_symbol head = symtable[index];
    S_symbol s;

    for(s = head; s; s = s->next) {
        if (!strcmp(s->name, name))
            return s;
    }

    s = U_alloc(sizeof(*s));
    s->name = U_strdup(name);
    s->next = head;

    symtable[index] = s;
    return symtable[index];
}

S_table S_mk_table(void)
{
    S_table t = U_alloc(sizeof(*t));

    memset(t, 0, sizeof(*t));

    return t;
}

const char *S_get_name(S_symbol s)
{
    return s->name;
}

void S_enter(S_table t, S_symbol s, void *v)
{
    unsigned index = ptrhash(s);
    bind b;

    if (!t || !s)
        U_error(-1, "enter fail");

    b = U_alloc(sizeof(*b));
    b->key      = s;
    b->value    = v;
    b->next     = t->binds[index];
    b->prevtop  = t->top;

    t->binds[index] = b;
    t->top = s;
}

void *S_look(S_table t, S_symbol s)
{
    unsigned index = ptrhash(s);
    bind b;

    if (!t || !s)
        U_error(-1, "look fail");

    for(b = t->binds[index]; b; b = b->next) {
        if (b->key == s)
            return b->value;
    }

    S_dump(t);
    return NULL;
}

void S_begin(S_table t, const char *name)
{
    char buf[64];

    if (!t)
        U_error(-1, "begin fail");

    // scope sign start with "!".
    snprintf(buf, sizeof(buf), "!%s", name);

    S_enter(t, S_mk_symbol(buf), NULL);
}

void S_end(S_table t)
{
    unsigned index;
    bind b;

    if (!t)
        U_error(-1, "end fail");

    // pop till scope sign.
    while (strncmp(S_get_name(t->top), "!", 1))
        S_pop(t);

    if (t->top)
        S_pop(t);
}
