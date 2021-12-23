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

AST_dec AST_mk_dec_var(Apos pos, SYM_symbol name, SYM_symbol type, AST_exp init)
{
    AST_dec p = UTL_alloc(sizeof(*p));

    p->kind         = AST_kind_dec_var;
    p->pos          = pos;
    p->u.var.name   = name;
    p->u.var.type   = type;
    p->u.var.init   = init;
    p->u.var.escape = true;

    return p;
}

AST_dec AST_mk_dec_type(SYM_symbol name, AST_type type)
{
    AST_dec p = UTL_alloc(sizeof(*p));

    p->kind        = AST_kind_dec_type;
    p->u.type.name = name;
    p->u.type.type = type;

    return p;
}

AST_dec AST_mk_dec_func(Apos pos, SYM_symbol name, AST_para_list paras,
                        SYM_symbol ret, AST_exp body)
{
    AST_dec p = UTL_alloc(sizeof(*p));

    p->kind         = AST_kind_dec_func;
    p->u.func.name  = name;
    p->u.func.paras = paras;
    p->u.func.ret   = ret;
    p->u.func.body  = body;

    return p;
}

/****************************************************************************
 * Public: expression constructor
 ****************************************************************************/

AST_exp AST_mk_exp_var(Apos pos, AST_var var)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind  = AST_kind_exp_var;
    p->pos   = pos;
    p->u.var = var;

    return p;
}

AST_exp AST_mk_exp_nil(Apos pos)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind = AST_kind_exp_nil;
    p->pos  = pos;

    return p;
}

AST_exp AST_mk_exp_int(Apos pos, int i)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind   = AST_kind_exp_int;
    p->pos    = pos;
    p->u.int_ = i;

    return p;
}

AST_exp AST_mk_exp_str(Apos pos, const char *s)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind   = AST_kind_exp_str;
    p->pos    = pos;
    p->u.str_ = s;

    return p;
}

AST_exp AST_mk_exp_call(Apos pos, SYM_symbol func, AST_exp_list args)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind        = AST_kind_exp_call;
    p->pos         = pos;
    p->u.call.func = func;
    p->u.call.args = args;

    return p;
}

AST_exp AST_mk_exp_op(Apos pos, AST_kind_op oper,
                      AST_exp left, AST_exp right)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind       = AST_kind_exp_op;
    p->pos        = pos;
    p->u.op.oper  = oper;
    p->u.op.left  = left;
    p->u.op.right = right;

    return p;
}

AST_exp AST_mk_exp_array(Apos pos, SYM_symbol type, AST_exp size,
                         AST_exp init)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind         = AST_kind_exp_array;
    p->pos          = pos;
    p->u.array.type = type;
    p->u.array.size = size;
    p->u.array.init = init;

    return p;
}

AST_exp AST_mk_exp_record(Apos pos, SYM_symbol type, AST_arg_list args)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind          = AST_kind_exp_record;
    p->pos           = pos;
    p->u.record.type = type;
    p->u.record.args = args;

    return p;
}

AST_exp AST_mk_exp_seq(Apos pos, AST_exp_list seq)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind  = AST_kind_exp_seq;
    p->pos   = pos;
    p->u.seq = seq;

    return p;
}

AST_exp AST_mk_exp_assign(Apos pos, AST_var var, AST_exp exp)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind         = AST_kind_exp_assign;
    p->pos          = pos;
    p->u.assign.var = var;
    p->u.assign.exp = exp;

    return p;
}

AST_exp AST_mk_exp_if(Apos pos, AST_exp cond, AST_exp then, AST_exp else_)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind        = AST_kind_exp_if;
    p->pos         = pos;
    p->u.if_.cond  = cond;
    p->u.if_.then  = then;
    p->u.if_.else_ = else_;

    return p;
}

AST_exp AST_mk_exp_while(Apos pos, AST_exp cond, AST_exp body)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind          = AST_kind_exp_while;
    p->pos           = pos;
    p->u.while_.cond = cond;
    p->u.while_.body = body;

    return p;
}

AST_exp AST_mk_exp_for(Apos pos, SYM_symbol var, AST_exp lo, AST_exp hi,
                       AST_exp body)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind          = AST_kind_exp_for;
    p->pos           = pos;
    p->u.for_.var    = var;
    p->u.for_.lo     = lo;
    p->u.for_.hi     = hi;
    p->u.for_.body   = body;
    p->u.for_.escape = true;

    return p;
}

AST_exp AST_mk_exp_break(Apos pos)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind = AST_kind_exp_break;
    p->pos  = pos;

    return p;
}

AST_exp AST_mk_exp_let(Apos pos, AST_dec_list decs, AST_exp_list body)
{
    AST_exp p = UTL_alloc(sizeof(*p));

    p->kind       = AST_kind_exp_let;
    p->pos        = pos;
    p->u.let.decs = decs;
    p->u.let.body = body;

    return p;
}

/****************************************************************************
 * Public: variables constructor
 ****************************************************************************/

AST_var AST_mk_var_base(Apos pos, SYM_symbol name, AST_var suffix)
{
    AST_var p = UTL_alloc(sizeof(*p));

    p->kind          = AST_kind_var_base;
    p->pos           = pos;
    p->u.base.name   = name;
    p->u.base.suffix = suffix;

    return p;
}

AST_var AST_mk_var_index(Apos pos, AST_exp exp, AST_var suffix)
{
    AST_var p = UTL_alloc(sizeof(*p));

    p->kind           = AST_kind_var_index;
    p->pos            = pos;
    p->u.index.exp    = exp;
    p->u.index.suffix = suffix;

    return p;
}

AST_var AST_mk_var_field(Apos pos, SYM_symbol field, AST_var suffix)
{
    AST_var p = UTL_alloc(sizeof(*p));

    p->kind           = AST_kind_var_field;
    p->pos            = pos;
    p->u.field.name   = field;
    p->u.field.suffix = suffix;

    return p;
}

/****************************************************************************
 * Public: type definition constructor
 ****************************************************************************/

AST_type AST_mk_type_name(Apos pos, SYM_symbol name)
{
    AST_type p = UTL_alloc(sizeof(*p));

    p->kind     = AST_kind_type_name;
    p->pos      = pos;
    p->u.name   = name;

    return p;
}

AST_type AST_mk_type_array(Apos pos, SYM_symbol array)
{
    AST_type p = UTL_alloc(sizeof(*p));

    p->kind     = AST_kind_type_array;
    p->pos      = pos;
    p->u.array  = array;

    return p;
}

AST_type AST_mk_type_record(Apos pos, AST_para_list fields)
{
    AST_type p = UTL_alloc(sizeof(*p));

    p->kind     = AST_kind_type_record;
    p->pos      = pos;
    p->u.record = fields;

    return p;
}

/****************************************************************************
 * Public: link list constructor
 ****************************************************************************/

AST_dec_list AST_mk_dec_list(AST_dec head, AST_dec_list tail)
{
    AST_dec_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

AST_exp_list AST_mk_exp_list(AST_exp head, AST_exp_list tail)
{
    AST_exp_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

AST_para_list AST_mk_para_list(AST_para head, AST_para_list tail)
{
    AST_para_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

AST_arg_list AST_mk_arg_list(AST_arg head, AST_arg_list tail)
{
    AST_arg_list p = UTL_alloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

AST_para AST_mk_para(Apos pos, SYM_symbol name, SYM_symbol type)
{
    AST_para p = UTL_alloc(sizeof(*p));

    p->pos      = pos;
    p->name     = name;
    p->type     = type;
    p->escape   = true;

    return p;
}

AST_arg AST_mk_arg(SYM_symbol name, AST_exp exp)
{
    AST_arg p = UTL_alloc(sizeof(*p));

    p->name = name;
    p->exp  = exp;

    return p;
}
