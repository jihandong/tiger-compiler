/****************************************************************************
 * Includes
 ****************************************************************************/

#include <string.h>
#include "table.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define TABLE_SIZE 109

typedef struct bind_ *bind;

struct bind_
{
    void *  key;
    void *  value;
    bind    next;       /*< next node in hash bucket */
    void *  prevtop;    /*< previous bind's key */
};

struct TAB_table_
{
    bind table[TABLE_SIZE];
    void *top;
};

/********************************************************************************
 * Private Functions
 ********************************************************************************/

static inline unsigned hash(void *key)
{
    return ((unsigned)key) % TABLE_SIZE;
}

static bind TAB_mk_bind(void *key, void* value, bind next, void *prevtop)
{
    bind b = UTL_alloc(sizeof(*b));

    b->key     = key;
    b->value   = value;
    b->next    = next;
    b->prevtop = prevtop;

    return b;
}

/********************************************************************************
 * Public Functions
 ********************************************************************************/

TAB_table TAB_empty(void)
{
    TAB_table t = UTL_alloc(sizeof(*t));

    memset(t, 0, sizeof(*t));

    return t;
}

void TAB_enter(TAB_table t, void *key, void *value)
{
    int index;

    if (!t)
        UTL_error(UTL_NOPOS, "enter to a null table");
    if (!key)
        UTL_error(UTL_NOPOS, "enter null key to table");

    index = hash(key);

    t->table[index] = TAB_mk_bind(key, value, t->table[index], t->top);
    t->top          = key;
}

void *TAB_look(TAB_table t, void *key)
{
    int index;
    bind b;

    if (!t)
        UTL_error(UTL_NOPOS, "look into a null table");
    if (!key)
        UTL_error(UTL_NOPOS, "look null key into table");

    index = hash(key);

    for (b = t->table[index]; b; b = b->next) {
        if (b->key == key)
            return b->value;
    }

    return NULL;
}

void *TAB_pop(TAB_table t)
{
    void *key;
    int index;
    bind b;

    if (!t)
        UTL_error(UTL_NOPOS, "pop a null table");

    key = t->top;
    if (!key)
        UTL_error(UTL_NOPOS, "pop a top-empty table");

    index = hash(key);
    b     = t->table[index];
    if (!b) // impossible
        UTL_error(UTL_NOPOS, "pop a buggy table");

    t->table[index] = b->next;
    t->top          = b->prevtop;

    return b->key;
}

void TAB_dump(TAB_table t, void (*show)(void *key, void *value))
{
    // ENOSYS
    (void)t;
    (void)show;
}
