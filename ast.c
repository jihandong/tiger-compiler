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

ast_dec ast_mk_dec_var(ast_pos pos, symbol var, symbol type, ast_exp init)
{
    ast_dec p = try_malloc(sizeof(*p));

    p->kind         = kind_dec_var;
    p->pos          = pos;
    p->u.var.var    = var;
    p->u.var.type   = type;
    p->u.var.init   = init;
    p->u.var.escape = true;

    return p;
}

ast_dec ast_mk_dec_type(symbol type_s, ast_type type)
{
    ast_dec p = try_malloc(sizeof(*p));

    p->kind             = kind_dec_type;
    p->u.type.type_s    = type_s;
    p->u.type.type      = type;

    return p;
}

ast_dec ast_mk_dec_func(ast_pos pos, symbol func, ast_tfield_list params, symbol ret, ast_exp body)
{
    ast_dec p = try_malloc(sizeof(*p));

    p->kind             = kind_dec_func;
    p->u.func.func      = func;
    p->u.func.params    = params;
    p->u.func.ret       = ret;
    p->u.func.body      = body;

    return p;
}

/****************************************************************************
 * Public Functions: expressions
 ****************************************************************************/

ast_exp ast_mk_exp_var(ast_pos pos, ast_var var)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind     = kind_exp_var;
    p->pos      = pos;
    p->u.var    = var;

    return p;
}

ast_exp ast_mk_exp_nil(ast_pos pos)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind     = kind_exp_nil;
    p->pos      = pos;

    return p;
}

ast_exp ast_mk_exp_int(ast_pos pos, int i)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind     = kind_exp_int;
    p->pos      = pos;
    p->u.int_   = i;

    return p;
}

ast_exp ast_mk_exp_string(ast_pos pos, const char *s)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind         = kind_exp_string;
    p->pos          = pos;
    p->u.string_    = s;

    return p;
}

ast_exp ast_mk_exp_call(ast_pos pos, symbol func, ast_exp_list args)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind         = kind_exp_call;
    p->pos          = pos;
    p->u.call.func  = func;
    p->u.call.args  = args;

    return p;
}

ast_exp ast_mk_exp_op(ast_pos pos, ast_op op, ast_exp left, ast_exp right)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind         = kind_exp_op;
    p->pos          = pos;
    p->u.op.op      = op;
    p->u.op.left    = left;
    p->u.op.right   = right;

    return p;
}

ast_exp ast_mk_exp_record(ast_pos pos, symbol record, ast_rfield_list members)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind             = kind_exp_record;
    p->pos              = pos;
    p->u.record.record  = record;
    p->u.record.members = members;

    return p;
}

ast_exp ast_mk_exp_seq(ast_pos pos, ast_exp_list seq)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind     = kind_exp_seq;
    p->pos      = pos;
    p->u.seq    = seq;

    return p;
}

ast_exp ast_mk_exp_assign(ast_pos pos, ast_var var, ast_exp exp)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind         = kind_exp_assign;
    p->pos          = pos;
    p->u.assign.var = var;
    p->u.assign.exp = exp;

    return p;
}

ast_exp ast_mk_exp_if(ast_pos pos, ast_exp cond, ast_exp then, ast_exp else_)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind         = kind_exp_if;
    p->pos          = pos;
    p->u.if_.cond   = cond;
    p->u.if_.then   = then;
    p->u.if_.else_  = else_;

    return p;
}

ast_exp ast_mk_exp_while(ast_pos pos, ast_exp cond, ast_exp body)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind             = kind_exp_while;
    p->pos              = pos;
    p->u.while_.cond    = cond;
    p->u.while_.body    = body;

    return p;
}

ast_exp ast_mk_exp_for(ast_pos pos, symbol var, ast_exp lo, ast_exp hi, ast_exp body)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind             = kind_exp_for;
    p->pos              = pos;
    p->u.for_.var       = var;
    p->u.for_.lo        = lo;
    p->u.for_.hi        = hi;
    p->u.for_.body      = body;
    p->u.for_.escape    = true;

    return p;
}

ast_exp ast_mk_exp_break(ast_pos pos)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind = kind_exp_break;
    p->pos  = pos;

    return p;
}

ast_exp ast_mk_exp_let(ast_pos pos, ast_dec_list decs, ast_exp_list body)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind         = kind_exp_let;
    p->pos          = pos;
    p->u.let.decs   = decs;
    p->u.let.body   = body;

    return p;
}

ast_exp ast_mk_exp_array(ast_pos pos, symbol array, ast_exp size, ast_exp init)
{
    ast_exp p = try_malloc(sizeof(*p));

    p->kind             = kind_exp_array;
    p->pos              = pos;
    p->u.array.array    = array;
    p->u.array.size     = size;
    p->u.array.init     = init;

    return p;
}

/****************************************************************************
 * Public Functions: variables
 ****************************************************************************/

ast_var ast_mk_var(ast_pos pos, symbol var)
{
    ast_var p = try_malloc(sizeof(*p));

    p->kind     = kind_var;
    p->pos      = pos;
    p->u.var    = var;

    return p;
}

ast_var ast_mk_var_slice(ast_pos pos, ast_var var, ast_exp exp)
{
    ast_var p = try_malloc(sizeof(*p));

    p->kind         = kind_var_slice;
    p->pos          = pos;
    p->u.slice.var  = var;
    p->u.slice.exp  = exp;

    return p;
}

ast_var ast_mk_var_member(ast_pos pos, ast_var var, symbol member)
{
    ast_var p = try_malloc(sizeof(*p));

    p->kind             = kind_var_member;
    p->pos              = pos;
    p->u.member.var     = var;
    p->u.member.member  = member;

    return p;
}

/****************************************************************************
 * Public Functions: types
 ****************************************************************************/

ast_type ast_mk_type_var(ast_pos pos, symbol var)
{
    ast_type p = try_malloc(sizeof(*p));

    p->kind     = kind_type_var;
    p->pos      = pos;
    p->u.var    = var;

    return p;
}

ast_type ast_mk_type_array(ast_pos pos, symbol array)
{
    ast_type p = try_malloc(sizeof(*p));

    p->kind     = kind_type_array;
    p->pos      = pos;
    p->u.array  = array;

    return p;
}

ast_type ast_mk_type_record(ast_pos pos, ast_tfield_list record)
{
    ast_type p = try_malloc(sizeof(*p));

    p->kind     = kind_type_record;
    p->pos      = pos;
    p->u.record = record;

    return p;
}

/****************************************************************************
 * Public Functions: link list
 ****************************************************************************/

ast_dec_list ast_mk_dec_list(ast_dec head, ast_dec_list tail)
{
    ast_dec_list p = try_malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_exp_list ast_mk_exp_list(ast_exp head, ast_exp_list tail)
{
    ast_exp_list p = try_malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_tfield_list ast_mk_tfield_list(ast_tfield head, ast_tfield_list tail)
{
    ast_tfield_list p = try_malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_rfield_list ast_mk_rfield_list(ast_rfield head, ast_rfield_list tail)
{
    ast_rfield_list p = try_malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_tfield ast_mk_tfield(ast_pos pos, symbol var, symbol type)
{
    ast_tfield p = try_malloc(sizeof(*p));

    p->pos      = pos;
    p->var      = var;
    p->type     = type;
    p->escape   = true;

    return p;
}

ast_rfield ast_mk_rfield(symbol var, ast_exp exp)
{
    ast_rfield p = try_malloc(sizeof(*p));

    p->var = var;
    p->exp = exp;

    return p;
}
