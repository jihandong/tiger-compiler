/********************************************************************************
 * Includes
 ********************************************************************************/

#include "ast.h"
#include "symbol.h"
#include "type.h"
#include "util.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

typedef void *ir_exp; /*< ir not implemented yet */

static struct sem_exp { ir_exp exp; ty_type type; };

/********************************************************************************
 * Privates
 ********************************************************************************/

static sem_exp sem_mk_exp(ir_exp exp, ty_type type)
{
    struct sem_exp e;

    e.exp   = exp;
    e.type  = type;

    return e;
}

/********************************************************************************
 * Publics
 ********************************************************************************/

void sem_trans_dec(symbol_table venv, symbol_table tenv, ast_var n)
{
    switch(n->kind) {
        case kind_dec_var: {
            struct sem_exp init = sem_trans_exp(venv, tenv, n->u.var.init);
            ty_type var_type;

            // require type match between 'type' and 'init'.
            if (n->u.var.type) {
                var_type = sym_table_get(tenv, n->u.var.type);
                if (init.type != var_type)
                    util_err(n->pos, "dec var: init exp's type not match");
            }
            sym_table_add(venv, n->u.var.var, init.type);
            return;
        }

        case kind_dec_type:
            sym_table_add(tenv, n->u.type.type_s, sem_trans_type(n->u.type.type));
            return;

        case kind_dec_func: {
            ty_type ret_type = sym_table_get(n->u.func.ret);
            ty_type_list arg_type_list = NULL; /* ast_tfield_list -> ty_type_list */
            ty_type_list t;
            ast_tfield_list a;

            sym_table_add(venv, n->u.func.func, ty_mk_func(ret_type, arg_type_list));

            // type check in function body.
            sym_table_begin(venv);

            for(a = n->u.func.params, t = arg_type_list; a && t; a = a->next, t = t->next)
                sym_table_add(venv, a->head->var, sym_table_get(tenv, a->head->type));
            sym_trans_exp(venv, tenv, n->u.func.body);

            sym_table_end(venv);
            // end of body
            return;
        }

        default:
    }
}

struct sem_exp sem_trans_exp(symbol_table venv, symbol_table tenv, ast_exp n)
{
    switch(n->kind) {
        case kind_exp_var:
            return sem_trans_var(venv, tenv, n->u.var);

        case kind_exp_nil:
            return sem_mk_exp(NULL, ty_mk_nil());

        case kind_exp_int:
            return sem_mk_exp(NULL, ty_mk_int());

        case kind_exp_string:
            return sem_mk_exp(NULL, ty_mk_string());

        case kind_exp_call:
            

        case kind_exp_op: {
            struct sem_exp left  = sem_trans_exp(venv, tenv, n->u.op.left);
            struct sem_exp right = sem_trans_exp(venv, tenv, n->u.op.right);

            if (left.type->kind != kind_ty_int)
                util_err(n->u.op.left->pos, "exp op: left exp's type not match");
            if (right.type->kind != kind_ty_int)
                util_err(n->u.op.right->pos, "exp op: right exp's type not match");

            return sem_mk_exp(NULL, ty_mk_int());
        }

        case kind_exp_record:
            

        case kind_exp_seq: {
            struct sem_exp last;
            ast_exp_list list;

            for(list = n->u.seq, last = NULL; list; list = list->next)
                last = sem_trans_exp(venv, tenv, list->head);

            return sem_mk_exp(NULL, last);
        }

        case kind_exp_assign:
            

        case kind_exp_if:
            struct sem_exp cond   = sem_trans_exp(venv, tenv, n->u.if_.cond);
            struct sem_exp then   = sem_trans_exp(venv, tenv, n->u.if_.then);
            struct sem_exp else_  = sem_trans_exp(venv, tenv, n->u.if_.else_);

            if (cond.type != kind_ty_int)
                util_err(n->u.op.left->pos, "exp if: cond exp's type isnot int");

            /* what should I return? */

        case kind_exp_while:
            

        case kind_exp_for:
            

        case kind_exp_break:
            return sem_mk_exp(NULL, ty_mk_void());

        case kind_exp_let: {
            struct sem_exp exp;
            ast_dec_list d;

            // prepare decs
            sym_table_begin(venv);
            sym_table_begin(tenv);

            for(d = n->u.let.decs; d; d = d->next)
                sem_trans_dec(venv, tenv, d->head);
            exp = sem_trans_exp(venv, tenv, n->u.let.body);

            sym_table_end(tenv);
            sym_table_end(venv);
            // end of exp

            return exp;
        }

        case kind_exp_array:
            

        default:
    }
}

struct sem_exp sem_trans_var(symbol_table venv, symbol_table tenv, ast_var n)
{
    switch(n->kind) {
        case:
        default:
    }
}

ty_type sem_exp sem_trans_type(symbol_table tenv, ast_type n)
{
    switch(n->kind) {
        case:
        default:
    }
}









