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
static struct T_type_ type_nil  = { T_kind_nil };
static struct T_type_ type_int  = { T_kind_int };
static struct T_type_ type_str  = { T_kind_str };
static struct T_type_ type_void = { T_kind_void };

static const char *T_kind[] =
{
    "nil",
    "int",
    "string",
    "void",
    "array",
    "record",
    "name",
};

/****************************************************************************
 * Public: basic type
 ****************************************************************************/

inline T_type T_nil(void)   { return &type_nil; }
inline T_type T_int(void)   { return &type_int; }
inline T_type T_str(void)   { return &type_str; }
inline T_type T_void(void)  { return &type_void; }

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

T_field T_mk_field(S_symbol field, T_type type)
{
    T_field t = U_alloc(sizeof(*t));

    t->field = field;
    t->type  = type;

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

void T_pr_type(T_type type)
{
    if (type) {
        printf("%s", T_kind[type->kind]);
        if (type->kind == T_kind_name)
            printf("(%s)", S_get_name(type->u.name.symbol));
    } else
        printf("_");
}

void T_pr_type_list(T_type_list types)
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
