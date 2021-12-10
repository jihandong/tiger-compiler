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

T_type T_mk_name(S_symbol symbol, T_type type)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind          = T_kind_name;
    t->u.name.symbol = symbol;
    t->u.name.type   = type;

    return t;
}

T_type T_mk_func(T_type ret, T_type_list paras)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind         = T_kind_func;
    t->u.func.ret   = ret;
    t->u.func.paras = paras;

    return t;
}

T_type T_mk_array(T_type type)
{
    T_type t = U_alloc(sizeof(*t));

    t->kind    = T_kind_array;
    t->u.array = type;

    return t;
}

T_type T_mk_record(T_field_list fields)
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

inline int T_get_kind(T_type t)
{
    if (!t)
        return -1;
    else if (t->kind == T_kind_name)
        return t->u.name.type->kind;
    else
        return t->kind;
}


inline bool T_match(T_type left, T_type right)
{
    if (left == right)
        return true;

    if (T_get_kind(left) == T_kind_record && T_get_kind(right) == T_kind_nil)
        return true;

    return false;
}
