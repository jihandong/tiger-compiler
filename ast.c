/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdbool.h>
#include "symbol.h"
#include "ast.h"
#include "util.h"

/****************************************************************************
 * Public Functions: declares
 ****************************************************************************/

A_dec Am_dec_var(Apos pos, S_symbol var, S_symbol type, A_exp init)
{
    A_dec p = Ualloc(sizeof(*p));

    p->kind         = Ak_dec_var;
    p->pos          = pos;
    p->u.var.var    = var;
    p->u.var.type   = type;
    p->u.var.init   = init;
    p->u.var.escape = true;

    return p;
}

A_dec Am_dec_type(S_symbol type_s, A_type type)
{
    A_dec p = Ualloc(sizeof(*p));

    p->kind             = Ak_dec_type;
    p->u.type.type_s    = type_s;
    p->u.type.type      = type;

    return p;
}

A_dec Am_dec_func(Apos pos, S_symbol func, A_para_list params, S_symbol ret, A_exp body)
{
    A_dec p = Ualloc(sizeof(*p));

    p->kind             = Ak_dec_func;
    p->u.func.func      = func;
    p->u.func.params    = params;
    p->u.func.ret       = ret;
    p->u.func.body      = body;

    return p;
}

/****************************************************************************
 * Public Functions: expressions
 ****************************************************************************/

A_exp Am_exp_var(Apos pos, A_var var)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind     = Ak_exp_var;
    p->pos      = pos;
    p->u.var    = var;

    return p;
}

A_exp Am_exp_nil(Apos pos)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind     = Ak_exp_nil;
    p->pos      = pos;

    return p;
}

A_exp Am_exp_int(Apos pos, int i)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind     = Ak_exp_int;
    p->pos      = pos;
    p->u.int_   = i;

    return p;
}

A_exp Am_exp_string(Apos pos, const char *s)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind         = Ak_exp_string;
    p->pos          = pos;
    p->u.string_    = s;

    return p;
}

A_exp Am_exp_call(Apos pos, S_symbol func, A_exp_list args)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind         = Ak_exp_call;
    p->pos          = pos;
    p->u.call.func  = func;
    p->u.call.args  = args;

    return p;
}

A_exp Am_exp_op(Apos pos, Ak_op op, A_exp left, A_exp right)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind         = Ak_exp_op;
    p->pos          = pos;
    p->u.op.oper    = op;
    p->u.op.left    = left;
    p->u.op.right   = right;

    return p;
}

A_exp Am_exp_record(Apos pos, S_symbol record, A_argu_list members)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind             = Ak_exp_record;
    p->pos              = pos;
    p->u.record.record  = record;
    p->u.record.members = members;

    return p;
}

A_exp Am_exp_seq(Apos pos, A_exp_list seq)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind     = Ak_exp_seq;
    p->pos      = pos;
    p->u.seq    = seq;

    return p;
}

A_exp Am_exp_assign(Apos pos, A_var var, A_exp exp)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind         = Ak_exp_assign;
    p->pos          = pos;
    p->u.assign.var = var;
    p->u.assign.exp = exp;

    return p;
}

A_exp Am_exp_if(Apos pos, A_exp cond, A_exp then, A_exp else_)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind         = Ak_exp_if;
    p->pos          = pos;
    p->u.if_.cond   = cond;
    p->u.if_.then   = then;
    p->u.if_.else_  = else_;

    return p;
}

A_exp Am_exp_while(Apos pos, A_exp cond, A_exp body)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind             = Ak_exp_while;
    p->pos              = pos;
    p->u.while_.cond    = cond;
    p->u.while_.body    = body;

    return p;
}

A_exp Am_exp_for(Apos pos, S_symbol var, A_exp lo, A_exp hi, A_exp body)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind             = Ak_exp_for;
    p->pos              = pos;
    p->u.for_.var       = var;
    p->u.for_.lo        = lo;
    p->u.for_.hi        = hi;
    p->u.for_.body      = body;
    p->u.for_.escape    = true;

    return p;
}

A_exp Am_exp_break(Apos pos)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind = Ak_exp_break;
    p->pos  = pos;

    return p;
}

A_exp Am_exp_let(Apos pos, A_dec_list decs, A_exp_list body)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind         = Ak_exp_let;
    p->pos          = pos;
    p->u.let.decs   = decs;
    p->u.let.body   = body;

    return p;
}

A_exp Am_exp_array(Apos pos, S_symbol array, A_exp size, A_exp init)
{
    A_exp p = Ualloc(sizeof(*p));

    p->kind             = Ak_exp_array;
    p->pos              = pos;
    p->u.array.array    = array;
    p->u.array.size     = size;
    p->u.array.init     = init;

    return p;
}

/****************************************************************************
 * Public Functions: variables
 ****************************************************************************/

A_var Am_var(Apos pos, S_symbol base, A_var suffix)
{
    A_var p = Ualloc(sizeof(*p));

    p->kind             = Ak_var;
    p->pos              = pos;
    p->u.base.base      = base;
    p->u.base.suffix    = suffix;

    return p;
}

A_var Am_var_slice(Apos pos, A_exp exp, A_var suffix)
{
    A_var p = Ualloc(sizeof(*p));

    p->kind             = Ak_var_slice;
    p->pos              = pos;
    p->u.slice.exp      = exp;
    p->u.slice.suffix   = suffix;

    return p;
}

A_var Am_var_member(Apos pos, S_symbol member, A_var suffix)
{
    A_var p = Ualloc(sizeof(*p));

    p->kind             = Ak_var_member;
    p->pos              = pos;
    p->u.member.member  = member;
    p->u.member.suffix  = suffix;

    return p;
}

/****************************************************************************
 * Public Functions: types
 ****************************************************************************/

A_type Am_type_var(Apos pos, S_symbol var)
{
    A_type p = Ualloc(sizeof(*p));

    p->kind     = Ak_type_var;
    p->pos      = pos;
    p->u.var    = var;

    return p;
}

A_type Am_type_array(Apos pos, S_symbol array)
{
    A_type p = Ualloc(sizeof(*p));

    p->kind     = Ak_type_array;
    p->pos      = pos;
    p->u.array  = array;

    return p;
}

A_type Am_type_record(Apos pos, A_para_list record)
{
    A_type p = Ualloc(sizeof(*p));

    p->kind     = Ak_type_record;
    p->pos      = pos;
    p->u.record = record;

    return p;
}

/****************************************************************************
 * Public Functions: link list
 ****************************************************************************/

A_dec_list Am_dec_list(A_dec head, A_dec_list tail)
{
    A_dec_list p = Ualloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_exp_list Am_exp_list(A_exp head, A_exp_list tail)
{
    A_exp_list p = Ualloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_para_list Am_para_list(A_para head, A_para_list tail)
{
    A_para_list p = Ualloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_argu_list Am_argu_list(A_argu head, A_argu_list tail)
{
    A_argu_list p = Ualloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

A_para Am_para(Apos pos, S_symbol var, S_symbol type)
{
    A_para p = Ualloc(sizeof(*p));

    p->pos      = pos;
    p->var      = var;
    p->type     = type;
    p->escape   = true;

    return p;
}

A_argu Am_argu(S_symbol var, A_exp exp)
{
    A_argu p = Ualloc(sizeof(*p));

    p->var = var;
    p->exp = exp;

    return p;
}
