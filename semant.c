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

typedef void *I_ir; /*< ir not implemented yet */

typedef struct T_transed_ T_transed;

struct T_transed_ {
    I_ir    ir;
    T_type  type;
};

/********************************************************************************
 * Privates
 ********************************************************************************/

static T_transed T_trans(I_ir expir, T_type type)
{
    T_transed e;

    e.ir    = ir;
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
                var_type = S_look(tenv, n->u.var.type);
                if (init.type != var_type)
                    util_err(n->pos, "dec var: init exp's type not match");
            }
            S_enter(venv, n->u.var.var, init.type);
            return;
        }

        case A_kind_dec_type:
            S_enter(tenv, n->u.type.type_s, T_trans_type(n->u.type.type));
            return;

        case A_kind_dec_func: {
            T_type ret_type = S_look(n->u.func.ret);
            T_type_list arg_type_list = NULL; /* A_para_list -> T_type_list */
            T_type_list t;
            A_para_list a;

            S_enter(venv, n->u.func.func, T_mk_func(ret_type, arg_type_list));

            // type check in function body.
            S_begin(venv);

            for(a = n->u.func.params, t = arg_type_list; a && t; a = a->next, t = t->next)
                S_enter(venv, a->head->var, S_look(tenv, a->head->type));
            sym_trans_exp(venv, tenv, n->u.func.body);

            S_end(venv);
            // end of body
            return;
        }

        default:
    }
}

T_transed T_trans_exp(S_table venv, S_table tenv, A_exp n)
{
    switch(n->kind) {
        case A_kind_exp_var:
            return T_trans_var(venv, tenv, n->u.var);

        case A_kind_exp_nil:
            return T_trans(NULL, T_nil());

        case A_kind_exp_int:
            return T_trans(NULL, T_int());

        case A_kind_exp_str:
            return T_trans(NULL, T_str());

        case A_kind_exp_call: {
            S_symbol   name = n->u.call.func;
            A_exp_list args = n->u.call.args;
            T_type     func = S_look(venv, name);

            if (!func)
                U_error(n->pos, "function not decalred");
            for(; args; args = args->tail)
                T_trans_exp(venv, tenv, args->head);

            return T_trans(NULL, func->u.func.ret);
        }

        case A_kind_exp_op: {
            A_exp     left  = n->u.op.left;
            A_exp     right = n->u.op.right;
            T_transed ltr   = T_trans_exp(venv, tenv, left);
            T_transed rtr   = T_trans_exp(venv, tenv, right);

            if (ltr.type->kind != T_kind_int)
                util_err(left->pos, "left operand is not integer type");
            if (rtr.type->kind != T_kind_int)
                util_err(right->pos, "right operand is not integer type");

            return T_trans(NULL, T_int());
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
            A_exp     cond  = n->u.if_.cond;
            A_exp     then  = n->u.if_.then;
            A_exp     else_ = n->u.if_.else_;
            T_transed ctr   = T_trans_exp(venv, tenv, cond);
            T_transed ttr   = T_trans_exp(venv, tenv, then);
            T_transed etr   = T_trans_exp(venv, tenv, else_);

            if (ctr.type != T_kind_int)
                util_err(cond->pos, "if condition is not integer type");
            if (ttr.type != etr.type)
                util_err(cond->pos, "if branches are not the same type");

            return T_trans(NULL, ttr.type);
        }

        case A_kind_exp_while:
            

        case A_kind_exp_for:
            

        case A_kind_exp_break:
            return T_trans(NULL, T_mk_void());

        case A_kind_exp_let: {
            T_exp exp;
            A_dec_list d;

            // prepare decs
            S_begin(venv);
            S_begin(tenv);

            for(d = n->u.let.decs; d; d = d->tail)
                T_trans_dec(venv, tenv, d->head);
            exp = T_trans_exp(venv, tenv, n->u.let.body);

            S_end(tenv);
            S_end(venv);
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









