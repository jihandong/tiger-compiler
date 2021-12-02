/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "util.h"

/****************************************************************************
 * Private Variables
 ****************************************************************************/

// basic types
static struct T_type_ type_nil     = { Tk_nil };
static struct T_type_ type_int     = { Tk_int };
static struct T_type_ type_string  = { Tk_string };
static struct T_type_ type_void    = { Tk_void };

static const char *Tk[] =
{
    "nil",
    "int",
    "string",
    "array",
    "record",
    "name",
    "void",
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

inline T_type Tf_nil(void)
{
    return &type_nil;
}

inline T_type Tf_int(void)
{
    return &type_int;
}

inline T_type Tf_string(void)
{
    return &type_string;
}

inline T_type Tf_void(void)
{
    return &type_void;
}

T_type Tf_name(S_symbol symbol, T_type type)
{
    T_type t = Ualloc(sizeof(*t));

    t->kind             = Tk_name;
    t->u.name.symbol    = symbol;
    t->u.name.type      = type;

    return t;
}

T_type Tf_func(T_type ret, T_type_list args)
{
    T_type t = Ualloc(sizeof(*t));

    t->kind         = Tk_func;
    t->u.func.ret   = ret;
    t->u.func.args  = args;

    return t;
}

T_type Tf_array(T_type type)
{
    T_type t = Ualloc(sizeof(*t));

    t->kind     = Tk_array;
    t->u.array  = type;

    return t;
}

T_type Tf_record(T_field_list members)
{
    T_type t = Ualloc(sizeof(*t));

    t->kind     = Tk_record;
    t->u.record = members;

    return t;
}

T_field Tf_field(S_symbol field, T_type type)
{
    T_field t = Ualloc(sizeof(*t));

    t->field = field;
    t->type  = type;

    return t;
}

T_type_list Tf_type_list(T_type head, T_type_list tail)
{
    T_type_list t = Ualloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

T_field_list Tf_field_list(T_field head, T_field_list tail)
{
    T_field_list t = Ualloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

void Tp_type(T_type type)
{
    if (type) {
        printf("%s", Tk[type->kind]);
        if (type->kind == Tk_name)
            printf("(%s)", sym_get_name(type->u.name.symbol));
    } else
        printf("_");
}

void Tp_type_list(T_type_list types)
{
    if (types) {
        printf("type_list(");
        T_pr_type(types->head);
        if (types->tail) {
            printf(",");
            T_pr_type_list(types->tail);
        }
        printf(")");
    } else
        printf("_");
}
