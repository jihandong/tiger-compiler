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

struct SYM_symbol_
{
    const char *name;
    SYM_symbol  next;
};

/********************************************************************************
 * Private Data
 ********************************************************************************/

static SYM_symbol symtable[SYM_TABLE_SIZE];

static struct SYM_symbol_ sign = { "<scope>", NULL, };

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

static SYM_symbol SYM_mk_symbol(const char *name, SYM_symbol next)
{
    SYM_symbol s = UTL_alloc(sizeof(*s));

    s->name = name;
    s->next = next;

    return s;
}

/********************************************************************************
 * Public Functions
 ********************************************************************************/

SYM_symbol SYM_declare(const char *name)
{
    unsigned index;
    SYM_symbol head, s;

    index = BKDRhash(name);
    head  = symtable[index];

    for (s = head; s; s = s->next) {
        if (!strcmp(s->name, name))
            return s; // symbol already exists.
    }

    symtable[index] = SYM_mk_symbol(name, head);
    return symtable[index];
}

const char *SYM_get_name(SYM_symbol s)
{
    return s->name;
}

SYM_table SYM_empty(void)
{
    return TAB_empty();
}

void SYM_enter(SYM_table t, SYM_symbol s, void *v)
{
    return TAB_enter(t, s, v);
}

void *SYM_look(SYM_table t, SYM_symbol s)
{
    return TAB_look(t, s);
}

void SYM_begin(SYM_table t)
{
    SYM_enter(t, &sign, NULL);
}

void SYM_end(SYM_table t)
{
    SYM_symbol s;

    do {
        s = TAB_pop(t);
    } while (s != &sign);
}

void SYM_dump(SYM_table t, void (*show)(SYM_symbol s, void *v))
{
    TAB_dump(t, (void (*)(void *, void *)) show);
}