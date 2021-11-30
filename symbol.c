/********************************************************************************
 * Include Files
 ********************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "symbol.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

#define HASH_TABLE_SIZE 109
#define BIND_TABLE_SIZE 109

typedef struct bind_*   bind;

struct bind_
{
    symbol  key;
    void *  value;
    bind    next;       /*< next node in hash bucket */
    symbol  prevtop;    /*< previous bound symbol */
};

struct symbol_
{
    const char *name;
    symbol      next;
};

struct symbol_table_
{
    bind binds[BIND_TABLE_SIZE];
    symbol top;     /*< newest bound symbol */
};

/********************************************************************************
 * Private Data
 ********************************************************************************/

static symbol hashtable[HASH_TABLE_SIZE];
static symbol empty = 0x000000001;

/********************************************************************************
 * Private Functions
 ********************************************************************************/

static inline unsigned BKDRhash(const char *s)
{
    unsigned seed = 131, hash = 0;

    while (*s)
        hash = hash * seed + (*s++);

    return hash % HASH_TABLE_SIZE;
}

static inline unsigned ptrhash(symbol *s)
{
    unsigned hash = s;

    return hash % HASH_TABLE_SIZE;
}

static inline symbol sym_mk(const char *name, symbol head)
{
    symbol s = try_malloc(sizeof(*s));

    s->name = try_strdup(name);
    s->next = head;

    return s;
}

static inline bind bind_mk(symbol key, void *value, bind head, symbol top)
{
    bind b = try_malloc(sizeof(*b));

    b->key      = key;
    b->value    = value;
    b->next     = head;
    b->prevtop  = top;

    return b;
}

/********************************************************************************
 * Public Functions
 ********************************************************************************/

symbol sym_get(const char *name)
{
    unsigned index = BKDRhash(name);
    symbol head = hashtable[index];
    symbol node;

    for(node = head; node; node = node->next) {
        if (!strcmp(node->name, name))
            return node;
    }

    hashtable[index] = sym_mk(name, head);
    return hashtable[index];
}

const char *sym_get_name(symbol s)
{
    return s->name;
}

symbol_table sym_table_new(void)
{
    symbol_table t = try_malloc(sizeof(*t));

    memset(t, 0, sizeof(*t));

    return t;
}

void sym_table_add(symbol_table t, symbol s, void *v)
{
    unsigned index = ptrhash(s);

    assert(t && s);
    t->binds[index] = bind_mk(s, v, t->binds[index], t->top);
    t->top = s;

    return t->binds[index];
}

void *sym_table_get(symbol_table t, symbol s)
{
    unsigned index = ptrhash(s);
    bind b;

    assert(t && s);
    for(b = t->binds[index]; b; b = b->next) {
        if (b->key == s)
            return b->value;
    }

    return NULL;
}

void sym_table_begin(symbol_table t)
{
    assert(t);
    sym_table_add(t, empty, NULL); // sign of scope begin
}

void sym_table_end(symbol_table t)
{
    unsigned index;
    bind b;

    assert(t);
    while(t->top != empty) {
        index           = ptrhash(t->top);
        b               = t->binds[index];
        t->binds[index] = b->next;
        t->top          = b->prevtop;
    }
}
