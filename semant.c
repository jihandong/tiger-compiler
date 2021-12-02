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

typedef void *I_exp; /*< ir not implemented yet */

typedef struct T_exp_ T_exp;

struct T_exp_ {
    I_exp  exp;
    T_type type;
};

/********************************************************************************
 * Privates
 ********************************************************************************/

static T_exp T_mk_exp(I_exp exp, T_type type)
{
    T_exp e;

    e.exp   = exp;
    e.type  = type;

    return e;
}

/********************************************************************************
 * Publics
 ********************************************************************************/

void T_trans_dec(S_table venv, S_table tenv, A_var n)
{
    switch(n->kind) {
        case A_kind_dec_var: {
            T_exp init = T_trans_exp(venv, tenv, n->u.var.init);
            T_type var_type;

            // require type match between 'type' and 'init'.
            if (n->u.var.type) {
                var_type = sym_table_get(tenv, n->u.var.type);
                if (init.type != var_type)
                    util_err(n->pos, "dec var: init exp's type not match");
            }
            sym_table_add(venv, n->u.var.var, init.type);
            return;
        }

        case A_kind_dec_type:
            sym_table_add(tenv, n->u.type.type_s, T_trans_type(n->u.type.type));
            return;

        case A_kind_dec_func: {
            T_type ret_type = sym_table_get(n->u.func.ret);
            T_type_list arg_type_list = NULL; /* A_para_list -> T_type_list */
            T_type_list t;
            A_para_list a;

            sym_table_add(venv, n->u.func.func, T_mk_func(ret_type, arg_type_list));

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

T_exp T_trans_exp(S_table venv, S_table tenv, A_exp n)
{
    switch(n->kind) {
        case A_kind_exp_var:
            return T_trans_var(venv, tenv, n->u.var);

        case A_kind_exp_nil:
            return T_mk_exp(NULL, T_mk_nil());

        case A_kind_exp_int:
            return T_mk_exp(NULL, T_mk_int());

        case A_kind_exp_string:
            return T_mk_exp(NULL, T_mk_string());

        case A_kind_exp_call:

        case A_kind_exp_op: {
            T_exp left  = T_trans_exp(venv, tenv, n->u.op.left);
            T_exp right = T_trans_exp(venv, tenv, n->u.op.right);

            if (left.type->kind != T_kind_int)
                util_err(n->u.op.left->pos, "exp op: left exp's type not match");
            if (right.type->kind != T_kind_int)
                util_err(n->u.op.right->pos, "exp op: right exp's type not match");

            return T_mk_exp(NULL, T_mk_int());
        }

        case A_kind_exp_record:
            

        case A_kind_exp_seq: {
            T_exp last;
            A_exp_list list;

            for(list = n->u.seq, last = NULL; list; list = list->tail)
                last = T_trans_exp(venv, tenv, list->head);

            return last;
        }

        case A_kind_exp_assign:
            

        case A_kind_exp_if:
            T_exp cond   = T_trans_exp(venv, tenv, n->u.if_.cond);
            T_exp then   = T_trans_exp(venv, tenv, n->u.if_.then);
            T_exp else_  = T_trans_exp(venv, tenv, n->u.if_.else_);

            if (cond.type != T_kind_int)
                util_err(n->u.op.left->pos, "exp if: cond exp's type isnot int");

            /* what should I return? */

        case A_kind_exp_while:
            

        case A_kind_exp_for:
            

        case A_kind_exp_break:
            return T_mk_exp(NULL, T_mk_void());

        case A_kind_exp_let: {
            T_exp exp;
            A_dec_list d;

            // prepare decs
            sym_table_begin(venv);
            sym_table_begin(tenv);

            for(d = n->u.let.decs; d; d = d->tail)
                T_trans_dec(venv, tenv, d->head);
            exp = T_trans_exp(venv, tenv, n->u.let.body);

            sym_table_end(tenv);
            sym_table_end(venv);
            // end of exp

            return exp;
        }

        case A_kind_exp_array:
            

        default:
    }
}

T_exp T_trans_var(S_table venv, S_table tenv, A_var n)
{
    switch(n->kind) {
        case:
        default:
    }
}

T_type T_exp T_trans_type(S_table tenv, A_type n)
{
    switch(n->kind) {
        case:
        default:
    }
}









