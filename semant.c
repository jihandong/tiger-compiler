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

typedef struct T_exp_ T_exp;

struct T_exp_ {
    I_ir    ir;
    T_type  type;
};

/********************************************************************************
 * Privates
 ********************************************************************************/

static T_exp T_trans(I_ir expir, T_type type)
{
    T_exp e;

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
            S_symbol name = n->u.var.name;
            S_symbol type = n->u.var.type;
            A_exp    init = n->u.var.init;
            T_type   type_analy;
            T_exp    init_trans;

            var_init = T_trans_exp(venv, tenv, init);
            if (type_) {
                var_type = S_look(tenv, type);
                if (var_init.type != var_type)
                    U_err(n->pos, "init and type not match in variable declare");
            }

            S_enter(venv, name, var_init.type);
            return;
        }

        case A_kind_dec_type: {
            S_symbol name = n->u.type.name;
            S_symbol type = n->u.type.type;
            T_type   type_type;

            type_type = T_trans_type(type);

            S_enter(tenv, name, type_type);
            return;
        }

        case A_kind_dec_func: {
            S_symbol    fname = n->u.func.name;
            A_para_list paras = n->u.func.paras;
            S_symbol    ret   = n->u.func.ret;
            A_exp       body  = n->u.func.body;
            T_type_list func_paras;
            T_type      func_ret;
            T_exp       func_body;
            T_type_list p;
            int         i;

            // check return type.
            ret_ty = S_look(ret);
            if (!ret_ty)
                U_err(n->pos, "return type not declared");

            // check parameter type.
            for (i = 0; paras; paras = paras->tail, i++) {
                S_symbol name = paras->head->name;
                S_symbol type = paras->head->type;
                T_type type_ty, para_ty;

                type_ty = S_look(tenv, type);
                if (!type_ty_)
                    U_err(n->pos, "parameter type not declared");

                // make a parameter list.
                para_ty = T_mk_name(name, type_ty);
                if (i == 0) {
                    para_tys = T_mk_type_list(para_ty, NULL);
                    p = para_tys;
                } else {
                    p->tail = T_mk_type_list(para_ty, NULL);
                    p = p->tail;
                }
            }

            S_enter(venv, fname, T_mk_func(ret_ty, para_tys));
            S_begin(venv);

            for(; para_tys; para_tys = para_tys->tail) {
                S_symbol symbol = para_tys->head->symbol;
                T_type   type   = para_tys->head->type;

                S_enter(venv, symbol, type);
            }
            body_tr = T_trans_exp(venv, tenv, body);

            S_end(venv);

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
            return T_trans(NULL, T_mk_nil());

        case A_kind_exp_int:
            return T_trans(NULL, T_mk_int());

        case A_kind_exp_str:
            return T_trans(NULL, T_mk_str());

        case A_kind_exp_call: {
            S_symbol   func = n->u.call.func;
            A_exp_list args = n->u.call.args;
            T_type func_ty, ret_ty;

            func_ty = S_look(venv, func);
            if (!func_ty)
                U_error(n->pos, "function not decalred");

            ret_ty = func_ty->u.func.ret;
            for(; args; args = args->tail)
                T_trans_exp(venv, tenv, args->head);

            return T_trans(NULL, ret_ty);
        }

        case A_kind_exp_op: {
            A_exp     left  = n->u.op.left;
            A_exp     right = n->u.op.right;
            T_exp left_tr, right_tr;

            left_tr = T_trans_exp(venv, tenv, left);
            if (left_tr.type->kind != T_kind_int)
                U_err(left->pos, "left operand is not integer type");

            right_tr = T_trans_exp(venv, tenv, right);
            if (right_tr.type->kind != T_kind_int)
                U_err(right->pos, "right operand is not integer type");

            return T_trans(NULL, T_int());
        }

        case A_kind_exp_array: {
            S_symbol  type = n->u.array.type;
            A_exp     size = n->u.array.size;
            A_exp     init = n->u.array.init;
            T_type    type_ty;
            T_exp size_tr, init_tr;

            type_ty = S_look(tenv, type);
            if (!type_ty)
                U_err(right->pos, "type in array create not exist");

            size_tr = T_trans_exp(venv, tenv, size);
            if (size_tr.type != T_kind_int)
                U_err(right->pos, "size in array create is not integer");

            init_tr = T_trans_exp(venv, tenv, init);
            if (init_tr.type != type)
                U_err(right->pos, "type and init not match in array create");

            return T_trans(NULL, T_mk_array(type_ty));
        }

        case A_kind_exp_record: {
            S_symbol    type   = n->u.record.type;
            A_argu_list fields = n->u.record.fields;
            T_type type_ty;

            type_ty = S_look(tenv, type);
            if (!type_ty)
                U_err(->pos, "type in record create not exist");

            /* check fields order and exp type */
        }

        case A_kind_exp_seq: {
            A_exp_list seq = n->u.seq;
            T_exp exp_tr;

            for(; seq; seq = seq->tail)
                exp_tr = T_trans_exp(venv, tenv, seq->head);

            return T_trans(NULL, exp_tr->type);
        }

        case A_kind_exp_assign:
            

        case A_kind_exp_if:
            A_exp cond  = n->u.if_.cond;
            A_exp then  = n->u.if_.then;
            A_exp else_ = n->u.if_.else_;
            T_exp cond_tr, then_tr, else_tr;

            cond_tr = T_trans_exp(venv, tenv, cond);
            if (cond_tr.type != T_kind_int)
                U_err(cond->pos, "if condition is not integer type");

            then_tr = T_trans_exp(venv, tenv, then);
            else_tr = T_trans_exp(venv, tenv, else_);
            if (then_tr.type != else_tr.type)
                U_err(cond->pos, "if branches are not the same type");

            return T_trans(NULL, then_tr.type);
        }

        case A_kind_exp_while: {
            A_exp cond = n->u.while_.cond;
            A_exp body = n->u.while_.body;
            T_exp cond_tr, body_tr;

            cond_tr = T_trans_exp(venv, tenv, cond);
            if (cond_tr.type != T_kind_int)
                U_err(cond->pos, "while condition is not integer type");

            S_begin(venv);
            body_tr = T_exp_exp(venv, tenv, body);
            S_end(venv);

            return T_exp(NULL, body_tr.type);
        }

        case A_kind_exp_for: {
            S_symbol var  = n->u.for_.var;
            A_exp    lo   = n->u.for_.lo;
            A_exp    hi   = n->u.for_.hi;
            A_exp    body = n->u.for_.body;
            T_exp lo_tr, hi_tr, body_tr;

            lo_tr = T_trans_exp(venv, tenv, lo);
            if (lo_tr.type.kind != T_kind_int)
                U_error(n->pos, "for lo is not integer type");

            hi_tr = T_trnas_exp(venv, tenv, hi);
            if (hi_tr.type.kind != T_kind_int)
                U_error(n->pos, "for hi is not integer type");

            S_begin(venv);
            S_enter(venv, var, T_mk_int());

            body_tr = T_trans_exp(venv, tenv, body);

            S_end(venv);

            return T_exp(NULL, body_tr.type);
        }

        case A_kind_exp_break:
            return T_trans(NULL, T_mk_void());

        case A_kind_exp_let: {
            A_dec_list decs = n->u.let.decs;
            A_exp_list body = n->u.let.body;
            T_exp  body_tr;

            S_begin(venv);
            S_begin(tenv);

            for(; decs; decs = decs->tail)
                T_trans_dec(venv, tenv, decs->head);
            for(; body; body = body->tail)
                body_tr = T_trans_exp(venv, tenv, body->head);

            S_end(tenv);
            S_end(venv);

            return T_trans(NULL, body_tr.type);
        }

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









