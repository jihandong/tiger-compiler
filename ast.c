/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include "symbol.h"
#include "ast.h"
#include "util.h"

/****************************************************************************
 * Public: declaration constructor
 ****************************************************************************/

A_dec A_mk_dec_var(Apos pos, S_symbol name, S_symbol type, A_exp init)
{
    A_dec p = U_alloc(sizeof(*p));

    p->kind         = A_kind_dec_var;
    p->pos          = pos;
    p->u.var.name   = name;
    p->u.var.type   = type;
    p->u.var.init   = init;
    p->u.var.escape = true;

    return p;
}

A_dec A_mk_dec_type(S_symbol name, A_type type)
{
    A_dec p = U_alloc(sizeof(*p));

    p->kind        = A_kind_dec_type;
    p->u.type.name = name;
    p->u.type.type = type;

    return p;
}

A_dec A_mk_dec_func(Apos pos, S_symbol name, A_para_list paras, S_symbol ret, A_exp body)
{
    A_dec p = U_alloc(sizeof(*p));

    p->kind         = A_kind_dec_func;
    p->u.func.name  = name;
    p->u.func.paras = paras;
    p->u.func.ret   = ret;
    p->u.func.body  = body;

    return p;
}

/****************************************************************************
 * Public: expression constructor
 ****************************************************************************/

A_exp A_mk_exp_var(Apos pos, A_var var)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind  = A_kind_exp_var;
    p->pos   = pos;
    p->u.var = var;

    return p;
}

A_exp A_mk_exp_nil(Apos pos)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind = A_kind_exp_nil;
    p->pos  = pos;

    return p;
}

A_exp A_mk_exp_int(Apos pos, int i)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind   = A_kind_exp_int;
    p->pos    = pos;
    p->u.int_ = i;

    return p;
}

A_exp A_mk_exp_str(Apos pos, const char *s)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind   = A_kind_exp_str;
    p->pos    = pos;
    p->u.str_ = s;

    return p;
}

A_exp A_mk_exp_call(Apos pos, S_symbol func, A_exp_list args)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind        = A_kind_exp_call;
    p->pos         = pos;
    p->u.call.func = func;
    p->u.call.args = args;

    return p;
}

A_exp A_mk_exp_op(Apos pos, A_kind_op oper, A_exp left, A_exp right)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind       = A_kind_exp_op;
    p->pos        = pos;
    p->u.op.oper  = oper;
    p->u.op.left  = left;
    p->u.op.right = right;

    return p;
}

A_exp A_mk_exp_array(Apos pos, S_symbol type, A_exp size, A_exp init)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind         = A_kind_exp_array;
    p->pos          = pos;
    p->u.array.type = type;
    p->u.array.size = size;
    p->u.array.init = init;

    return p;
}

A_exp A_mk_exp_record(Apos pos, S_symbol type, A_arg_list args)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind          = A_kind_exp_record;
    p->pos           = pos;
    p->u.record.type = type;
    p->u.record.args = args;

    return p;
}

A_exp A_mk_exp_seq(Apos pos, A_exp_list seq)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind  = A_kind_exp_seq;
    p->pos   = pos;
    p->u.seq = seq;

    return p;
}

A_exp A_mk_exp_assign(Apos pos, A_var var, A_exp exp)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind         = A_kind_exp_assign;
    p->pos          = pos;
    p->u.assign.var = var;
    p->u.assign.exp = exp;

    return p;
}

A_exp A_mk_exp_if(Apos pos, A_exp cond, A_exp then, A_exp else_)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind        = A_kind_exp_if;
    p->pos         = pos;
    p->u.if_.cond  = cond;
    p->u.if_.then  = then;
    p->u.if_.else_ = else_;

    return p;
}

A_exp A_mk_exp_while(Apos pos, A_exp cond, A_exp body)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind          = A_kind_exp_while;
    p->pos           = pos;
    p->u.while_.cond = cond;
    p->u.while_.body = body;

    return p;
}

A_exp A_mk_exp_for(Apos pos, S_symbol var, A_exp lo, A_exp hi, A_exp body)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind          = A_kind_exp_for;
    p->pos           = pos;
    p->u.for_.var    = var;
    p->u.for_.lo     = lo;
    p->u.for_.hi     = hi;
    p->u.for_.body   = body;
    p->u.for_.escape = true;

    return p;
}

A_exp A_mk_exp_break(Apos pos)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind = A_kind_exp_break;
    p->pos  = pos;

    return p;
}

A_exp A_mk_exp_let(Apos pos, A_dec_list decs, A_exp_list body)
{
    A_exp p = U_alloc(sizeof(*p));

    p->kind       = A_kind_exp_let;
    p->pos        = pos;
    p->u.let.decs = decs;
    p->u.let.body = body;

    return p;
}

/****************************************************************************
 * Public: variables constructor
 ****************************************************************************/

A_var A_mk_var_base(Apos pos, S_symbol name, A_var suffix)
{
    A_var p = U_alloc(sizeof(*p));

    p->kind          = A_kind_var_base;
    p->pos           = pos;
    p->u.base.name   = name;
    p->u.base.suffix = suffix;

    return p;
}

A_var A_mk_var_index(Apos pos, A_exp exp, A_var suffix)
{
    A_var p = U_alloc(sizeof(*p));

    p->kind           = A_kind_var_index;
    p->pos            = pos;
    p->u.index.exp    = exp;
    p->u.index.suffix = suffix;

    return p;
}

A_var A_mk_var_field(Apos pos, S_symbol field, A_var suffix)
{
    A_var p = U_alloc(sizeof(*p));

    p->kind           = A_kind_var_field;
    p->pos            = pos;
    p->u.field.name   = field;
    p->u.field.suffix = suffix;

    return p;
}

/****************************************************************************
 * Public: type definition constructor
 ****************************************************************************/

A_type A_mk_type_name(Apos pos, S_symbol name)
{
    A_type p = U_alloc(sizeof(*p));

    p->kind     = A_kind_type_name;
    p->pos      = pos;
    p->u.name   = name;

    return p;
}

A_type A_mk_type_array(Apos pos, S_symbol array)
{
    A_type p = U_alloc(sizeof(*p));

    p->kind     = A_kind_type_array;
    p->pos      = pos;
    p->u.array  = array;

    return p;
}

A_type A_mk_type_record(Apos pos, A_para_list fields)
{
    A_type p = U_alloc(sizeof(*p));

    p->kind     = A_kind_type_record;
    p->pos      = pos;
    p->u.record = fields;

    return p;
}

/****************************************************************************
 * Public: link list constructor
 ****************************************************************************/

A_dec_list A_mk_dec_list(A_dec head, A_dec_list tail)
{
    A_dec_list p = U_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_exp_list A_mk_exp_list(A_exp head, A_exp_list tail)
{
    A_exp_list p = U_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_para_list A_mk_para_list(A_para head, A_para_list tail)
{
    A_para_list p = U_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_arg_list A_mk_arg_list(A_arg head, A_arg_list tail)
{
    A_arg_list p = U_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_para A_mk_para(Apos pos, S_symbol name, S_symbol type)
{
    A_para p = U_alloc(sizeof(*p));

    p->pos      = pos;
    p->name     = name;
    p->type     = type;
    p->escape   = true;

    return p;
}

A_arg A_mk_arg(S_symbol name, A_exp exp)
{
    A_arg p = U_alloc(sizeof(*p));

    p->name = name;
    p->exp  = exp;

    return p;
}

