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
static struct ty_type_ ty_nil     = { kind_ty_nil };
static struct ty_type_ ty_int     = { kind_ty_int };
static struct ty_type_ ty_string  = { kind_ty_string };
static struct ty_type_ ty_void    = { kind_ty_void };

static const char *kind_ty[] =
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

inline ty_type ty_mk_nil(void)
{
    return &ty_nil;
}

inline ty_type ty_mk_int(void)
{
    return &ty_int;
}

inline ty_type ty_mk_string(void)
{
    return &ty_string;
}

inline ty_type ty_mk_void(void)
{
    return &ty_void;
}

ty_type ty_mk_name(symbol name, ty_type type)
{
    ty_type t = try_malloc(sizeof(*t));

    t->kind         = kind_ty_name;
    t->u.name.name  = name;
    t->u.name.type  = type;

    return t;
}

ty_type ty_mk_func(ty_type ret, ty_type_list args)
{
    ty_type t = try_malloc(sizeof(*t));

    t->kind         = kind_ty_func;
    t->u.func.ret   = ret;
    t->u.func.args  = args;

    return t;
}

ty_type ty_mk_array(ty_type type)
{
    ty_type t = try_malloc(sizeof(*t));

    t->kind     = kind_ty_array;
    t->u.array  = type;

    return t;
}

ty_type ty_mk_record(ty_rfield_list members)
{
    ty_type t = try_malloc(sizeof(*t));

    t->kind     = kind_ty_record;
    t->u.record = members;

    return t;
}

ty_rfield ty_mk_rfield(symbol name, ty_type type)
{
    ty_rfield t = try_malloc(sizeof(*t));

    t->name = name;
    t->type = type;

    return t;
}

ty_type_list ty_mk_type_list(ty_type head, ty_type_list tail)
{
    ty_type_list t = try_malloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

ty_rfield_list ty_mk_rfield_list(ty_rfield head, ty_rfield_list tail)
{
    ty_rfield_list t = try_malloc(sizeof(*t));

    t->head = head;
    t->tail = tail;

    return t;
}

void ty_pr_type(ty_type type)
{
    if (type) {
        printf("%s", kind_ty[type->kind]);
        if (type->kind == kind_ty_name)
            printf("(%s)", sym_get_name(type->u.name.name));
    } else
        printf("_");
}

void ty_pr_type_list(ty_type_list types)
{
    if (types) {
        printf("type_list(");
        ty_pr_type(types->head);
        if (types->tail) {
            printf(",");
            ty_pr_type_list(types->tail);
        }
        printf(")");
    } else
        printf("_");
}
