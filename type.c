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

static const char *TY_type_name[] =
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

TY_type TY_nil(void)
{
    static struct TY_type_ type_nil  = { TY_kind_nil };

    return &type_nil;
}

TY_type TY_int(void)
{
    static struct TY_type_ type_int  = { TY_kind_int };

    return &type_int;
}

TY_type TY_str(void)
{
    static struct TY_type_ type_str  = { TY_kind_str };

    return &type_str;
}

TY_type TY_void(void)
{
    static struct TY_type_ type_void = { TY_kind_void };

    return &type_void;
}


/****************************************************************************
 * Public: type constructor
 ****************************************************************************/

TY_type TY_mk_name(SYM_symbol symbol, TY_type type)
{
    TY_type t = UTL_alloc(sizeof(*t));

    t->kind          = TY_kind_name;
    t->u.name.symbol = symbol;
    t->u.name.type   = type;

    return t;
}

TY_type TY_mk_func(TY_type ret, TY_type_list paras)
{
    TY_type t = UTL_alloc(sizeof(*t));

    t->kind         = TY_kind_func;
    t->u.func.ret   = ret;
    t->u.func.paras = paras;

    return t;
}

TY_type TY_mk_array(TY_type type)
{
    TY_type t = UTL_alloc(sizeof(*t));

    t->kind    = TY_kind_array;
    t->u.array = type;

    return t;
}

TY_type TY_mk_record(TY_field_list fields)
{
    TY_type t = UTL_alloc(sizeof(*t));

    t->kind     = TY_kind_record;
    t->u.record = fields;

    return t;
}

/****************************************************************************
 * Public: parameter/argument constructor
 ****************************************************************************/

TY_field TY_mk_field(SYM_symbol name, TY_type type)
{
    TY_field t = UTL_alloc(sizeof(*t));

    t->name = name;
    t->type = type;

    return t;
}

TY_type_list TY_mk_type_list(TY_type head, TY_type_list tail)
{
    TY_type_list t = UTL_alloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

TY_field_list TY_mk_field_list(TY_field head, TY_field_list tail)
{
    TY_field_list t = UTL_alloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

/****************************************************************************
 * Public: display functions
 ****************************************************************************/

inline int TY_get_kind(TY_type t)
{
    if (!t)
        return -1;
    else if (t->kind == TY_kind_name)
        return t->u.name.type->kind;
    else
        return t->kind;
}


inline bool TY_match(TY_type left, TY_type right)
{
    if (left == right)
        return true;

    if (TY_get_kind(left) == TY_kind_record && TY_get_kind(right) == TY_kind_nil)
        return true;

    return false;
}
