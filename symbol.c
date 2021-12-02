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
    S_symbol  key;
    void *  value;
    bind    next;       /*< next node in hash bucket */
    S_symbol  prevtop;    /*< previous bound symbol */
};

struct S_symbol_
{
    const char *name;
    S_symbol      next;
};

struct S_table_
{
    bind binds[BIND_TABLE_SIZE];
    S_symbol top;     /*< newest bound S_symbol */
};

/********************************************************************************
 * Private Data
 ********************************************************************************/

static S_symbol hashtable[HASH_TABLE_SIZE];
static S_symbol empty = 0x000000001;

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

static inline unsigned ptrhash(S_symbol s)
{
    unsigned hash = s;

    return hash % HASH_TABLE_SIZE;
}

static inline S_symbol Sf_symbol_(const char *name, S_symbol head)
{
    S_symbol s = Ualloc(sizeof(*s));

    s->name = Ustrdup(name);
    s->next = head;

    return s;
}

static inline bind Sf_bind(S_symbol key, void *value, bind head, S_symbol top)
{
    bind b = Ualloc(sizeof(*b));

    b->key      = key;
    b->value    = value;
    b->next     = head;
    b->prevtop  = top;

    return b;
}

/********************************************************************************
 * Public Functions
 ********************************************************************************/

S_symbol Sf_symbol(const char *name)
{
    unsigned index = BKDRhash(name);
    S_symbol head = hashtable[index];
    S_symbol node;

    for(node = head; node; node = node->next) {
        if (!strcmp(node->name, name))
            return node;
    }

    hashtable[index] = Sf_symbol_(name, head);
    return hashtable[index];
}

S_table Sf_table(void)
{
    S_table t = Ualloc(sizeof(*t));

    memset(t, 0, sizeof(*t));

    return t;
}

const char *Sf_name(S_symbol s)
{
    return s->name;
}

void Sf_enter(S_table t, S_symbol s, void *v)
{
    unsigned index = ptrhash(s);

    assert(t && s);
    t->binds[index] = Sf_bind(s, v, t->binds[index], t->top);
    t->top = s;
}

void *Sf_look(S_table t, S_symbol s)
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

void Sf_begin(S_table t)
{
    assert(t);
    Sf_enter(t, empty, NULL); // sign of scope begin
}

void Sf_end(S_table t)
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
