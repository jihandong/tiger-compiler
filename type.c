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

static const char *TYP_type_name[] =
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

TYP_type TYP_nil(void)
{
    static struct TYP_type_ type_nil  = { TYP_kind_nil };

    return &type_nil;
}

TYP_type TYP_int(void)
{
    static struct TYP_type_ type_int  = { TYP_kind_int };

    return &type_int;
}

TYP_type TYP_str(void)
{
    static struct TYP_type_ type_str  = { TYP_kind_str };

    return &type_str;
}

TYP_type TYP_void(void)
{
    static struct TYP_type_ type_void = { TYP_kind_void };

    return &type_void;
}


/****************************************************************************
 * Public: type constructor
 ****************************************************************************/

TYP_type TYP_mk_name(SYM_symbol symbol, TYP_type type)
{
    TYP_type t = UTL_alloc(sizeof(*t));

    t->kind          = TYP_kind_name;
    t->u.name.symbol = symbol;
    t->u.name.type   = type;

    return t;
}

TYP_type TYP_mk_func(TYP_type ret, TYP_type_list paras)
{
    TYP_type t = UTL_alloc(sizeof(*t));

    t->kind         = TYP_kind_func;
    t->u.func.ret   = ret;
    t->u.func.paras = paras;

    return t;
}

TYP_type TYP_mk_array(TYP_type type)
{
    TYP_type t = UTL_alloc(sizeof(*t));

    t->kind    = TYP_kind_array;
    t->u.array = type;

    return t;
}

TYP_type TYP_mk_record(TYP_field_list fields)
{
    TYP_type t = UTL_alloc(sizeof(*t));

    t->kind     = TYP_kind_record;
    t->u.record = fields;

    return t;
}

/****************************************************************************
 * Public: parameter/argument constructor
 ****************************************************************************/

TYP_field TYP_mk_field(SYM_symbol name, TYP_type type)
{
    TYP_field t = UTL_alloc(sizeof(*t));

    t->name = name;
    t->type = type;

    return t;
}

TYP_type_list TYP_mk_type_list(TYP_type head, TYP_type_list tail)
{
    TYP_type_list t = UTL_alloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

TYP_field_list TYP_mk_field_list(TYP_field head, TYP_field_list tail)
{
    TYP_field_list t = UTL_alloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

/****************************************************************************
 * Public: display functions
 ****************************************************************************/

inline int TYP_get_kind(TYP_type t)
{
    if (!t)
        return -1;
    else if (t->kind == TYP_kind_name)
        return t->u.name.type->kind;
    else
        return t->kind;
}


inline bool TYP_match(TYP_type left, TYP_type right)
{
    if (left == right)
        return true;

    if (TYP_get_kind(left) == TYP_kind_record && TYP_get_kind(right) == TYP_kind_nil)
        return true;

    return false;
}
