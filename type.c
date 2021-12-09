/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "util.h"

/****************************************************************************
 * Private 
 ****************************************************************************/

static const char *T_type_name[] =
{
    "nil",
    "int",
    "str",
    "void",
    "name",
    "func",
    "array",
    "record",
};

/****************************************************************************
 * Public: basic type
 ****************************************************************************/

T_type T_nil(void)
{
    static struct T_type_ type_nil  = { T_kind_nil };

    return &type_nil;
}

T_type T_int(void)
{
    static struct T_type_ type_int  = { T_kind_int };

    return &type_int;
}

T_type T_str(void)
{
    static struct T_type_ type_str  = { T_kind_str };

    return &type_str;
}

T_type T_void(void)
{
    static struct T_type_ type_void = { T_kind_void };

    return &type_void;
}


/****************************************************************************
 * Public: type constructor
 ****************************************************************************/

T_type T_name(S_symbol symbol, T_type type)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind          = T_kind_name;
    t->u.name.symbol = symbol;
    t->u.name.type   = type;

    return t;
}

T_type T_func(T_type ret, T_type_list paras)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind         = T_kind_func;
    t->u.func.ret   = ret;
    t->u.func.paras = paras;

    return t;
}

T_type T_array(T_type type)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind    = T_kind_array;
    t->u.array = type;

    return t;
}

T_type T_record(T_field_list fields)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind     = T_kind_record;
    t->u.record = fields;

    return t;
}

/****************************************************************************
 * Public: parameter/argument constructor
 ****************************************************************************/

T_field T_mk_field(S_symbol name, T_type type)
{
    T_field t = U_alloc(sizeof(*t));

    t->name = name;
    t->type = type;

    return t;
}

T_type_list T_mk_type_list(T_type head, T_type_list tail)
{
    T_type_list t = U_alloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

T_field_list T_mk_field_list(T_field head, T_field_list tail)
{
    T_field_list t = U_alloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

/****************************************************************************
 * Public: display functions
 ****************************************************************************/

const char *T_get_name(T_type t)
{
    switch(t->kind) {
        case T_kind_nil:
        case T_kind_int:
        case T_kind_str:
        case T_kind_void:
        case T_kind_func:
        case T_kind_array:
        case T_kind_record:
            return T_type_name[t->kind];

        case T_kind_name:
            return T_get_name(t->u.name.type);

        default:
            U_error(-1, "unkown type");
    }
}

inline bool T_is(T_type t, int k)
{
    if (!t)
        U_error(-1, "empty type definition");

    if (t->kind == T_kind_name)
        return T_is(t->u.name.type, k);
    else
        return t->kind == k;
}
