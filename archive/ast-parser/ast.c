
#include <stdbool.h>

#include "symbol.h"
#include "ast.h"

/****************************************************************************
 * variables
 ****************************************************************************/

ast_var mk_ast_var_trival(ast_pos pos, symbol sym)
{
    ast_var p = malloc(sizeof(*p));

    p->kind     = kind_ast_var_trival;
    p->pos      = pos;
    p->u.trival = sym;

    return p;
}

ast_var mk_ast_var_field(ast_pos pos, ast_var var, symbol sym)
{
    ast_var p = malloc(sizeof(*p));

    p->kind         = kind_ast_var_field;
    p->pos          = pos;
    p->u.field.var  = var;
    p->u.field.sym  = sym;

    return p;
}

ast_var mk_ast_var_subscript(ast_pos pos, ast_var var, ast_exp exp)
{
    ast_var p = malloc(sizeof(*p));

    p->kind             = kind_ast_var_subscript;
    p->pos              = pos;
    p->u.subscript.var  = var;
    p->u.subscript.exp  = exp;

    return p;
}

/****************************************************************************
 * expressions
 ****************************************************************************/

ast_exp mk_ast_exp_var(ast_pos pos, ast_var var)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind     = kind_ast_exp_var;
    p->pos      = pos;
    p->u.var    = var;

    return p;
}

ast_exp mk_ast_exp_nil(ast_pos pos)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind     = kind_ast_exp_nil;
    p->pos      = pos;

    return p;
}

ast_exp mk_ast_exp_int(ast_pos pos, int i)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind     = kind_ast_exp_int;
    p->pos      = pos;
    p->u.int_   = i;

    return p;
}

ast_exp mk_ast_exp_string(ast_pos pos, const char *s)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind         = kind_ast_exp_string;
    p->pos          = pos;
    p->u.string_    = s;

    return p;
}

ast_exp mk_ast_exp_call(ast_pos pos, symbol func, ast_exp_list args)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind         = kind_ast_exp_call;
    p->pos          = pos;
    p->u.call.func  = func;
    p->u.call.args  = args;

    return p;
}

ast_exp mk_ast_exp_op(ast_pos pos, ast_op op, ast_exp left, ast_exp right)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind         = kind_ast_exp_call;
    p->pos          = pos;
    p->u.op.op      = op;
    p->u.op.left    = left;
    p->u.op.right   = right;

    return p;
}

ast_exp mk_ast_exp_record(ast_pos pos, symbol name, ast_record_list fields)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind             = kind_ast_exp_record;
    p->pos              = pos;
    p->u.record.name    = name;
    p->u.record.fields  = fields;

    return p;
}

ast_exp mk_ast_exp_seq(ast_pos pos, ast_exp_list seq)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind     = kind_ast_exp_seq;
    p->pos      = pos;
    p->u.seq    = seq;

    return p;
}

ast_exp mk_ast_exp_assign(ast_pos pos, ast_var var, ast_exp exp)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind         = kind_ast_exp_assign;
    p->pos          = pos;
    p->u.assign.var = var;
    p->u.assign.exp = exp;

    return p;
}

ast_exp mk_ast_exp_if(ast_pos pos, ast_exp cond, ast_exp then, ast_exp else_)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind         = kind_ast_exp_if;
    p->pos          = pos;
    p->u.if_.cond   = cond;
    p->u.if_.then   = then;
    p->u.if_.else_  = else_;

    return p;
}

ast_exp mk_ast_exp_while(ast_pos pos, ast_exp cond, ast_exp body)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind             = kind_ast_exp_while;
    p->pos              = pos;
    p->u.while_.cond    = cond;
    p->u.while_.body    = body;

    return p;
}

ast_exp mk_ast_exp_for(ast_pos pos, symbol var, ast_exp lo, ast_exp hi, ast_exp body)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind             = kind_ast_exp_for;
    p->pos              = pos;
    p->u.for_.var       = var;
    p->u.for_.lo        = lo;
    p->u.for_.hi        = hi;
    p->u.for_.body      = body;
    p->u.for_.escape    = true;

    return p;
}

ast_exp mk_ast_exp_break(ast_pos pos)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind             = kind_ast_exp_break;
    p->pos              = pos;

    return p;
}

ast_exp mk_ast_exp_let(ast_pos pos, ast_dec_list decs, ast_exp body)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind             = kind_ast_exp_let;
    p->pos              = pos;
    p->u.let.decs       = decs;
    p->u.let.body       = body;

    return p;
}

ast_exp mk_ast_exp_array(ast_pos pos, symbol name, ast_exp size, ast_exp init)
{
    ast_exp p = malloc(sizeof(*p));

    p->kind             = kind_ast_exp_array;
    p->pos              = pos;
    p->u.array.name     = name;
    p->u.array.size     = size;
    p->u.array.init     = init;

    return p;
}

/****************************************************************************
 * declares
 ****************************************************************************/

ast_dec mk_ast_dec_function(ast_pos pos, ast_func_dec_list function)
{
    ast_dec p = malloc(sizeof(*p));

    p->kind         = kind_ast_dec_function;
    p->pos          = pos;
    p->u.function   = function;

    return p;
}

ast_dec mk_ast_dec_type(ast_pos pos, ast_type_dec_list type)
{
    ast_dec p = malloc(sizeof(*p));

    p->kind     = kind_ast_dec_type;
    p->pos      = pos;
    p->u.type   = type;

    return p;
}

ast_dec mk_ast_dec_var(ast_pos pos, symbol var, symbol name, ast_exp init)
{
    ast_dec p = malloc(sizeof(*p));

    p->kind         = kind_ast_dec_var;
    p->pos          = pos;
    p->u.var.var    = var;
    p->u.var.name   = name;
    p->u.var.init   = init;
    p->u.var.escape = true;

    return p;
}

/****************************************************************************
 * types
 ****************************************************************************/

ast_type mk_ast_type_name(ast_pos pos, symbol name)
{
    ast_type p = malloc(sizeof(*p));

    p->kind     = kind_ast_dec_var;
    p->pos      = pos;
    p->u.name   = name;

    return p;
}

ast_type mk_ast_type_array(ast_pos pos, symbol array)
{
    ast_type p = malloc(sizeof(*p));

    p->kind     = kind_ast_dec_var;
    p->pos      = pos;
    p->u.array  = array;

    return p;
}

ast_type mk_ast_type_record(ast_pos pos, ast_arg_list record)
{
    ast_type p = malloc(sizeof(*p));

    p->kind     = kind_ast_dec_var;
    p->pos      = pos;
    p->u.record = record;

    return p;
}

/****************************************************************************
 * Link List
 ****************************************************************************/

ast_func_dec mk_ast_func_dec(ast_pos pos, symbol name, ast_arg_list params,
                             symbol result, ast_exp body)
{
    ast_func_dec p = checked_malloc(sizeof(*p));

    p->pos      = pos;
    p->name     = name;
    p->params   = params;
    p->result   = result;
    p->body     = body;

    return p;
}

ast_type_dec mk_ast_type_dec(symbol name, ast_type type)
{
    ast_type_dec p = checked_malloc(sizeof(*p));

    p->name = name;
    p->type = type;

    return p;
}

ast_arg mk_ast_arg(ast_pos pos, symbol name, symbol type_name)
{
    ast_arg p = checked_malloc(sizeof(*p));

    p->pos          = pos;
    p->name         = name;
    p->type_name    = type_name;
    p->escape       = true;

    return p;
}

ast_record mk_ast_record(symbol name, ast_exp exp)
{
    ast_record p = checked_malloc(sizeof(*p));

    p->name = name;
    p->exp  = exp;

    return p;
}

ast_exp_list mk_ast_exp_list(ast_exp head, ast_exp_list tail)
{
    ast_exp_list p = malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_dec_list mk_ast_dec_list(ast_dec head, ast_dec_list tail)
{
    ast_dec_list p = malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_func_dec_list mk_ast_func_dec_list(ast_func_dec head, ast_func_dec_list tail)
{
    ast_func_dec_list p = malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_type_dec_list mk_ast_type_dec_list(ast_type_dec head, ast_type_dec_list tail)
{
    ast_type_dec_list p = malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_arg_list mk_ast_arg_list(ast_arg head, ast_arg_list tail)
{
    ast_arg_list p = malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

ast_record_list mk_ast_record_list(ast_record head, ast_record_list tail)
{
    ast_record_list p = malloc(sizeof(*p));

    p->head = head;
    p->tail = tail;

    return p;
}

