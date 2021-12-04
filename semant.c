/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stddef.h>
#include "ast.h"
#include "symbol.h"
#include "type.h"
#include "util.h"

/****************************************************************************
 * Definition
 ****************************************************************************/

typedef void *I_ir; /*< ir not implemented yet */

typedef struct T_tyir_ T_tyir; /*< ir with type */

struct T_tyir_ {
    I_ir    ir;
    T_type  type;
};

void T_trans_dec(S_table venv, S_table tenv, A_dec n);
T_tyir T_trans_exp(S_table venv, S_table tenv, A_exp n);
T_tyir T_trans_var(S_table venv, S_table tenv, A_var n);
T_type T_trans_type(S_table tenv, A_type n);

/****************************************************************************
 * Private
 ****************************************************************************/

static T_tyir T_mk_tyir(I_ir ir, T_type type)
{
    T_tyir e;

    e.ir    = ir;
    e.type  = type;

    return e;
}

/****************************************************************************
 * Public
 ****************************************************************************/

void T_trans_dec(S_table venv, S_table tenv, A_dec n)
{
    switch(n->kind) {
        case A_kind_dec_var: {
            S_symbol name = n->u.var.name;
            S_symbol type = n->u.var.type;
            A_exp    init = n->u.var.init;
            T_type   type_ty;
            T_tyir   init_tyir;

            // check variable init.
            init_tyir = T_trans_exp(venv, tenv, init);
            if (type) {
                type_ty = S_look(tenv, type);
                if (init_tyir.type != type_ty)
                    U_error(n->pos, "init and type not match in var dec");
            }

            S_enter(venv, name, init_tyir.type);
            return;
        }

        case A_kind_dec_type: {
            S_symbol name = n->u.type.name;
            A_type   type = n->u.type.type;
            T_type   type_ty;

            type_ty = T_trans_type(tenv, type);

            S_enter(tenv, name, type_ty);
            return;
        }

        case A_kind_dec_func: {
            S_symbol    fname = n->u.func.name;
            A_para_list paras = n->u.func.paras;
            S_symbol    ret   = n->u.func.ret;
            A_exp       body  = n->u.func.body;

            T_type_list para_tys;
            T_type      ret_ty;
            T_tyir      body_tyir;

            A_para_list p;
            T_type_list t;

            // check return type.
            ret_ty = S_look(tenv, ret);
            if (!ret_ty)
                U_error(n->pos, "return type not exist in func dec");

            // check parameter type.
            for (p = paras, t = NULL; p; p = p->tail, t = t->tail) {
                S_symbol name = paras->head->name;
                S_symbol type = paras->head->type;
                T_type type_ty, para_ty;

                type_ty = S_look(tenv, type);
                if (!type_ty)
                    U_error(n->pos, "parameter type not exist in func dec");

                // make parameter list.
                para_ty = T_name(name, type_ty);
                t = T_mk_type_list(para_ty, NULL);
                if (p == paras) // keep head.
                    para_tys = t;
            }

            // check function body.
            S_enter(venv, fname, T_func(ret_ty, para_tys));
            S_begin(venv);

            for(p = paras, t = para_tys; p && t; p = p->tail, t = t->tail) {
                S_symbol name = p->head->name;
                T_type   type = t->head;

                S_enter(venv, name, type);
            }
            body_tyir = T_trans_exp(venv, tenv, body);

            S_end(venv);

            return;
        }

        default:
            U_error(n->pos, "unkown declaration");
    }
}

T_tyir T_trans_exp(S_table venv, S_table tenv, A_exp n)
{
    switch(n->kind) {
        case A_kind_exp_var:
            return T_trans_var(venv, tenv, n->u.var);

        case A_kind_exp_nil:
            return T_mk_tyir(NULL, T_nil());

        case A_kind_exp_int:
            return T_mk_tyir(NULL, T_int());

        case A_kind_exp_str:
            return T_mk_tyir(NULL, T_str());

        case A_kind_exp_call: {
            S_symbol    func = n->u.call.func;
            A_exp_list  args = n->u.call.args;

            T_type      func_ty, ret_ty;
            T_type_list para_tys;

            // check funtion.
            func_ty = S_look(venv, func);
            if (!func_ty)
                U_error(n->pos, "function not exist");

            ret_ty = func_ty->u.func.ret;

            // check parameter type.
            para_tys = func_ty->u.func.paras;
            for(; args && para_tys; args     = args->tail,
                                    para_tys = para_tys->tail) {
                A_exp  arg     = args->head;
                T_type para_ty = para_tys->head;
                T_tyir arg_tyir;

                arg_tyir = T_trans_exp(venv, tenv, arg);
                if (arg_tyir.type != para_ty)
                    U_error(n->pos, "type of para and arg not match");
            }

            return T_mk_tyir(NULL, ret_ty);
        }

        case A_kind_exp_op: {
            A_exp  left  = n->u.op.left;
            A_exp  right = n->u.op.right;
            T_tyir left_tyir, right_tyir;

            // check left operand
            left_tyir = T_trans_exp(venv, tenv, left);
            if (left_tyir.type->kind != T_kind_int)
                U_error(left->pos, "left operand is not integer type");

            // check right operand
            right_tyir = T_trans_exp(venv, tenv, right);
            if (right_tyir.type->kind != T_kind_int)
                U_error(right->pos, "right operand is not integer type");

            return T_mk_tyir(NULL, T_int());
        }

        case A_kind_exp_array: {
            S_symbol type = n->u.array.type;
            A_exp    size = n->u.array.size;
            A_exp    init = n->u.array.init;
            T_type   array_ty;
            T_tyir   size_tyir, init_tyir;

            // check array type.
            array_ty = S_look(tenv, type);
            if (!array_ty)
                U_error(n->pos, "array type not exist");
            if (array_ty->kind != T_kind_array)
                U_error(n->pos, "array type is not array");

            // check array size.
            size_tyir = T_trans_exp(venv, tenv, size);
            if (size_tyir.type->kind != T_kind_int)
                U_error(size->pos, "array size is not integer");

            // check array init.
            init_tyir = T_trans_exp(venv, tenv, init);
            if (init_tyir.type != array_ty->u.array)
                U_error(init->pos, "type of init and array not match");

            return T_mk_tyir(NULL, array_ty);
        }

        case A_kind_exp_record: {
            S_symbol     type = n->u.record.type;
            A_arg_list   args = n->u.record.args;
            T_type       record_ty;
            T_field_list fields;

            // check record type.
            record_ty = S_look(tenv, type);
            if (!record_ty)
                U_error(n->pos, "record type not exist");
            fields = record_ty->u.record;

            // check fields type.
            for(; args && fields; args = args->tail, fields = fields->tail) {
                S_symbol name1 = args->head->name;
                A_exp    exp   = args->head->exp;
                S_symbol name2 = fields->head->name;
                T_type   type  = fields->head->type;
                T_tyir   exp_tyir;

                if (name1 != name2)
                    U_error(exp->pos, "field name not match");

                exp_tyir = T_trans_exp(venv, tenv, exp);
                if (exp_tyir.type != type)
                    U_error(exp->pos, "field type not match");
            }

            return T_mk_tyir(NULL, record_ty);
        }

        case A_kind_exp_seq: {
            A_exp_list seq = n->u.seq;
            T_tyir     exp_tyir;

            for(; seq; seq = seq->tail)
                exp_tyir = T_trans_exp(venv, tenv, seq->head);

            return T_mk_tyir(NULL, exp_tyir.type);
        }

        case A_kind_exp_assign: {
            A_var  var = n->u.assign.var;
            A_exp  exp = n->u.assign.exp;
            T_tyir var_tyir, exp_tyir;

            // check type match.
            var_tyir = T_trans_var(venv, tenv, var);
            exp_tyir = T_trans_exp(venv, tenv, exp);
            if (var_tyir.type != exp_tyir.type)
                U_error(n->pos, "type of var and val not match");

            return T_mk_tyir(NULL, var_tyir.type);
        }

        case A_kind_exp_if: {
            A_exp  cond  = n->u.if_.cond;
            A_exp  then  = n->u.if_.then;
            A_exp  else_ = n->u.if_.else_;
            T_tyir cond_tyir, then_tyir, else_tyir;

            // check condition type.
            cond_tyir = T_trans_exp(venv, tenv, cond);
            if (cond_tyir.type->kind != T_kind_int)
                U_error(cond->pos, "cond type is not integer");

            // check branches type.
            then_tyir = T_trans_exp(venv, tenv, then);
            else_tyir = T_trans_exp(venv, tenv, else_);
            if (then_tyir.type != else_tyir.type)
                U_error(n->pos, "branches type are not the same");

            return T_mk_tyir(NULL, then_tyir.type);
        }

        case A_kind_exp_while: {
            A_exp  cond = n->u.while_.cond;
            A_exp  body = n->u.while_.body;
            T_tyir cond_tyir, body_tyir;

            // check condition type.
            cond_tyir = T_trans_exp(venv, tenv, cond);
            if (cond_tyir.type->kind != T_kind_int)
                U_error(cond->pos, "while condition is not integer type");

            S_begin(venv);
            body_tyir = T_trans_exp(venv, tenv, body);
            S_end(venv);

            return T_mk_tyir(NULL, body_tyir.type);
        }

        case A_kind_exp_for: {
            S_symbol var  = n->u.for_.var;
            A_exp    lo   = n->u.for_.lo;
            A_exp    hi   = n->u.for_.hi;
            A_exp    body = n->u.for_.body;
            T_tyir   lo_tyir, hi_tyir, body_tyir;

            // check lowest exp type.
            lo_tyir = T_trans_exp(venv, tenv, lo);
            if (lo_tyir.type->kind != T_kind_int)
                U_error(n->pos, "for lo is not integer type");

            // check highest exp type.
            hi_tyir = T_trans_exp(venv, tenv, hi);
            if (hi_tyir.type->kind != T_kind_int)
                U_error(n->pos, "for hi is not integer type");

            S_begin(venv);
            S_enter(venv, var, T_int());

            body_tyir = T_trans_exp(venv, tenv, body);

            S_end(venv);

            return T_mk_tyir(NULL, body_tyir.type);
        }

        case A_kind_exp_break:
            return T_mk_tyir(NULL, T_void());

        case A_kind_exp_let: {
            A_dec_list decs = n->u.let.decs;
            A_exp_list body = n->u.let.body;
            T_tyir     body_tyir;

            S_begin(venv);
            S_begin(tenv);

            for(; decs; decs = decs->tail)
                T_trans_dec(venv, tenv, decs->head);
            for(; body; body = body->tail)
                body_tyir = T_trans_exp(venv, tenv, body->head);

            S_end(tenv);
            S_end(venv);

            return T_mk_tyir(NULL, body_tyir.type);
        }

        default:
            U_error(n->pos, "unkown expression");
    }
}

T_tyir T_trans_var(S_table venv, S_table tenv, A_var n)
{
    S_symbol base;
    A_var    suffix;
    T_type   base_ty;
    T_tyir   var_tyir;
    A_var    p;
    T_type   t;

    // check base.
    if (n->kind != A_kind_var_base)
        U_error(n->pos, "lvalue has no adddress");

    // check symbol.
    base    = n->u.base.name;
    suffix  = n->u.base.suffix;
    base_ty = S_look(venv, base);
    if (!base_ty)
        U_error(n->pos, "lvalue not exist");

    for(p = suffix, t = base_ty; p;) {
        switch(suffix->kind) {
            case A_kind_var_index: {
                A_exp exp    = p->u.index.exp;
                A_var suffix = p->u.index.suffix;

                // check array type.
                if (t->kind != T_kind_array)
                    U_error(suffix->pos, "lvalue is not array");

                // check index type.
                if (exp->kind != T_kind_int)
                    U_error(exp->pos, "array index is not integer");

                // update.
                p = suffix;
                t = t->u.array;
                break;
            }

            case A_kind_var_field: {
                S_symbol     name   = p->u.field.name;
                A_var        suffix = p->u.index.suffix;
                T_field_list fields;
                T_field      field;

                // check record type.
                if (t->kind != T_kind_record)
                    U_error(suffix->pos, "lvalue is not record");

                // check field name.
                fields = t->u.record;
                for(field = NULL; fields; fields = fields->tail) {
                    if (fields->head->name == name) {
                        field = fields->head;
                        break;
                    }
                }
                if (!field)
                    U_error(suffix->pos, "field name not exist");

                // update.
                p = suffix;
                t = field->type;
                break;
            }

            case A_kind_var_base:
                U_error(suffix->pos, "lvalue has two bases?");

            default :
                U_error(n->pos, "unkown left value");
        }
    }

    return T_mk_tyir(NULL, t);
}

T_type T_trans_type(S_table tenv, A_type n)
{
    switch(n->kind) {
        case A_kind_type_name: {
            S_symbol name = n->u.name;
            T_type name_ty;

            // check type name.
            name_ty = S_look(tenv, name);
            if(!name_ty)
                U_error(n->pos, "type not exist");

            return name_ty;
        }

        case A_kind_type_array: {
            S_symbol array = n->u.array;
            T_type array_ty;

            // check element type name.
            array_ty = S_look(tenv, array);
            if(!array_ty)
                U_error(n->pos, "array type not exist");

            return T_array(array_ty);
        }

        case A_kind_type_record: {
            A_para_list  record = n->u.record;
            T_field_list fields;
            T_field_list p;
            int          i;

            // make field list.
            for(i = 0, p = NULL; record; record = record->tail) {
                S_symbol name = record->head->name;
                S_symbol type = record->head->type;
                T_type type_ty;
                T_field field;

                type_ty = S_look(tenv, type);
                if (!type_ty)
                    U_error(n->pos, "field type not exist");

                field = T_mk_field(name, type_ty);
                if (i != 0) {
                    fields = T_mk_field_list(field, NULL);
                    p = fields;
                } else {
                    p->tail = T_mk_field_list(field, NULL);
                    p = p->tail;
                }
            }

            return T_record(fields);
        }

        default:
            U_error(n->pos, "unkown type definition");
    }
}

