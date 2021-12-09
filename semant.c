/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stddef.h>
#include "semant.h"
#include "symbol.h"
#include "util.h"

/****************************************************************************
 * Private
 ****************************************************************************/

static void T_trans_dec(S_table venv, S_table tenv, A_dec_list n);
static T_tyir T_trans_exp(S_table venv, S_table tenv, A_exp n);
static T_tyir T_trans_var(S_table venv, S_table tenv, A_var n);
static T_type T_trans_type(S_table tenv, A_type n);

static T_tyir T_mk_tyir(I_ir ir, T_type type)
{
    T_tyir e;

    e.ir    = ir;
    e.type  = type;

    return e;
}

static void T_trans_dec(S_table venv, S_table tenv, A_dec_list decs)
{
    T_type_list dummys;
    A_dec_list  p;
    T_type_list t;

    // advertise type.
    for(p = decs, dummys = NULL; p; p = p->tail) {
        A_dec dec = p->head;

        switch(dec->kind) {
            case A_kind_dec_type: {
                S_symbol name  = dec->u.type.name;
                T_type   dummy = T_name(name, NULL);

                // advertise type but no definiton yet.
                S_enter(tenv, name, dummy);

                if (!dummys) {
                    t = T_mk_type_list(dummy, NULL);
                    dummys = t;
                } else {
                    t->tail = T_mk_type_list(dummy, NULL);
                    t = t->tail;
                }
                break;
            }

            case A_kind_dec_var:
            case A_kind_dec_func:
                break;

            default:
                U_error(-1, "X1");
        }
    }

    // parse type defitnions
    for(p = decs; p; p = p->tail) {
        A_dec dec = p->head;

        switch(dec->kind) {
            case A_kind_dec_type: {
                S_symbol name = dec->u.type.name;
                A_type   type = dec->u.type.type;
                T_type   type_ty = T_trans_type(tenv, type);

                // cover dummy with raw definitions
                S_enter(tenv, name, type_ty);
            }

            case A_kind_dec_var:
            case A_kind_dec_func:
                break;

            default:
                U_error(-1, "X2");
        }
    }

    // replace dummys with true definitons
    for(t = dummys; t; t = t->tail) {
        T_type   dummy  = t->head;
        S_symbol symbol = dummy->u.name.symbol;
        S_symbol s      = symbol;

        while(1) {
            T_type look = S_look(tenv, s);

            if (look->kind == T_kind_name) {
                S_symbol lsymbol = look->u.name.symbol;
                T_type   ltype   = look->u.name.type;

                if (lsymbol == symbol)
                    U_error(-1, "LOOP");

                s = lsymbol;
            } else {
                dummy->u.name.type = look;
                break;
            }
        }
    }

    // parse var definition and func definitions
    for(p = decs; p; p = p->tail) {
        A_dec dec = p->head;

        switch(dec->kind) {
            case A_kind_dec_var: {
                S_symbol name = dec->u.var.name;
                S_symbol type = dec->u.var.type;
                A_exp    init = dec->u.var.init;
                T_type   type_ty;
                T_tyir   init_tyir;

                // check variable init.
                init_tyir = T_trans_exp(venv, tenv, init);
                if (type) {
                    type_ty = S_look(tenv, type);
                    if (init_tyir.type != type_ty)
                        U_error(dec->pos, "variable has type(%s) but init with(%s)",
                                T_get_name(type_ty), T_get_name(init_tyir.type));
                }

                S_enter(venv, name, init_tyir.type);
                break;
            }

            case A_kind_dec_type:
                break;

            case A_kind_dec_func: {
                S_symbol    fname = dec->u.func.name;
                A_para_list paras = dec->u.func.paras;
                S_symbol    ret   = dec->u.func.ret;
                A_exp       body  = dec->u.func.body;

                T_type_list para_tys;
                T_type      ret_ty;
                T_tyir      body_tyir;

                A_para_list p;
                T_type_list t;

                // check return type.
                if (ret) {
                    ret_ty = S_look(tenv, ret);
                    if (!ret_ty)
                        U_error(dec->pos, "return type(%s) not defined",
                                S_get_name(ret));
                } else
                    ret_ty = T_void();

                // check parameter type.
                for (p = paras, para_tys = NULL; p; p = p->tail) {
                    S_symbol type = p->head->type;
                    T_type para_ty;

                    para_ty = S_look(tenv, type);
                    if (!para_ty)
                        U_error(dec->pos, "parameter type(%s) nod defined",
                                S_get_name(type));

                    // make parameter list.
                    if (!para_tys) {
                        t = T_mk_type_list(para_ty, NULL);
                        para_tys = t;
                    } else {
                        t->tail = T_mk_type_list(para_ty, NULL);
                        t = t->tail;
                    }
                }

                S_enter(venv, fname, T_func(ret_ty, para_tys));
                break;
            }

            default:
                U_error(-1, "X3");
        }
    }

    // parse func bodies
    for(p = decs; p; p = p->tail) {
        A_dec dec = p->head;

        switch(dec->kind) {
            case A_kind_dec_var:
            case A_kind_dec_type:
                break;

            case A_kind_dec_func: {
                S_symbol    fname = dec->u.func.name;
                A_para_list paras = dec->u.func.paras;
                A_exp       body  = dec->u.func.body;

                T_type      func     = S_look(venv, fname);
                T_type_list para_tys = func->u.func.paras;
                T_tyir      body_tyir;

                A_para_list p;
                T_type_list t;

                S_begin(venv, S_get_name(fname));

                for(p = paras, t = para_tys; p && t; p = p->tail, t = t->tail) {
                    S_symbol name = p->head->name;
                    T_type   type = t->head;

                    S_enter(venv, name, type);
                }
                body_tyir = T_trans_exp(venv, tenv, body);

                S_end(venv);

                break;
            }

            default:
                U_error(-1, "X4");
        }
    }
}

static T_tyir T_trans_exp(S_table venv, S_table tenv, A_exp n)
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
                U_error(n->pos, "func(%s) not exist", S_get_name(func));

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
                    U_error(n->pos, "func(%s), para(%s), arg(%s)",
                            S_get_name(func),
                            T_get_name(para_ty),
                            T_get_name(arg_tyir.type));
            }

            return T_mk_tyir(NULL, ret_ty);
        }

        case A_kind_exp_op: {
            A_exp  left  = n->u.op.left;
            A_exp  right = n->u.op.right;
            T_tyir left_tyir, right_tyir;

            // check left operand
            left_tyir = T_trans_exp(venv, tenv, left);
            if (T_get_kind(left_tyir.type) != T_kind_int)
                U_error(left->pos, "left operand has type(%s)",
                        T_get_name(left_tyir.type));

            // check right operand
            right_tyir = T_trans_exp(venv, tenv, right);
            if (T_get_kind(right_tyir.type) != T_kind_int)
                U_error(right->pos, "right operand has type(%s)",
                        T_get_name(right_tyir.type));

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
            if (T_get_kind(array_ty) != T_kind_array)
                U_error(n->pos, "type(%s) is not array",
                        T_get_name(array_ty));

            // check array size.
            size_tyir = T_trans_exp(venv, tenv, size);
            if (T_get_kind(size_tyir.type) != T_kind_int)
                U_error(size->pos, "array size has type(%s)",
                        T_get_name(size_tyir.type));

            // check array init.
            init_tyir = T_trans_exp(venv, tenv, init);
            if (init_tyir.type != array_ty->u.array)
                U_error(init->pos, "array element type(%s) but init(%s)",
                        T_get_name(array_ty->u.array),
                        T_get_name(init_tyir.type));

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
                    U_error(exp->pos, "name(%s) and field(%s) not match",
                            S_get_name(name1), S_get_name(name2));

                exp_tyir = T_trans_exp(venv, tenv, exp);
                if (exp_tyir.type != type)
                    U_error(exp->pos, "type(%s) and field(%s) not match",
                            T_get_name(exp_tyir.type), T_get_name(type));
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
                U_error(n->pos, "assign variable(%s) with type(%s)",
                                T_get_name(var_tyir.type),
                                T_get_name(exp_tyir.type));

            return T_mk_tyir(NULL, var_tyir.type);
        }

        case A_kind_exp_if: {
            A_exp  cond  = n->u.if_.cond;
            A_exp  then  = n->u.if_.then;
            A_exp  else_ = n->u.if_.else_;
            T_tyir cond_tyir, then_tyir, else_tyir;

            // check condition type.
            cond_tyir = T_trans_exp(venv, tenv, cond);
            if (T_get_kind(cond_tyir.type) != T_kind_int)
                U_error(cond->pos, "if-cond has type(%s)",
                        T_get_name(cond_tyir.type));

            // check branches type.
            then_tyir = T_trans_exp(venv, tenv, then);
            if (else_) {
                else_tyir = T_trans_exp(venv, tenv, else_);
                if (then_tyir.type != else_tyir.type)
                    U_error(n->pos, "then has type(%s), else_ has type(%s)",
                            T_get_name(then_tyir.type),
                            T_get_name(else_tyir.type));
            }

            return T_mk_tyir(NULL, then_tyir.type);
        }

        case A_kind_exp_while: {
            A_exp  cond = n->u.while_.cond;
            A_exp  body = n->u.while_.body;
            T_tyir cond_tyir, body_tyir;

            // check condition type.
            cond_tyir = T_trans_exp(venv, tenv, cond);
            if (T_get_kind(cond_tyir.type) != T_kind_int)
                U_error(cond->pos, "while-cond has type(%s)",
                        T_get_name(cond_tyir.type));

            S_begin(venv, "while");
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
            if (T_get_kind(lo_tyir.type) != T_kind_int)
                U_error(n->pos, "for-lo has type(%s)",
                        T_get_name(lo_tyir.type));

            // check highest exp type.
            hi_tyir = T_trans_exp(venv, tenv, hi);
            if (T_get_kind(hi_tyir.type) != T_kind_int)
                U_error(n->pos, "for-hi has type(%s)",
                        T_get_name(hi_tyir.type));

            S_begin(venv, "for");
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

            S_begin(venv, "let");
            S_begin(tenv, "let");

            T_trans_dec(venv, tenv, decs);
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

static T_tyir T_trans_var(S_table venv, S_table tenv, A_var n)
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
                A_exp  exp      = p->u.index.exp;
                A_var  suffix   = p->u.index.suffix;
                T_tyir exp_tyir = T_trans_exp(venv, tenv, exp);

                // check array type.
                if (T_get_kind(t) != T_kind_array)
                    U_error(suffix->pos, "lvalue is not array, but type(%s)",
                            T_get_name(t));

                // check index type.
                if (T_get_kind(exp_tyir.type) != T_kind_int)
                    U_error(exp->pos, "lvalue array index has type(%s)",
                            T_get_name(exp_tyir.type));

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
                if (T_get_kind(t) != T_kind_record)
                    U_error(suffix->pos, "lvalue is not record, but type(%s)",
                            T_get_name(t));

                // check field name.
                fields = t->u.record;
                for(field = NULL; fields; fields = fields->tail) {
                    if (fields->head->name == name) {
                        field = fields->head;
                        break;
                    }
                }
                if (!field)
                    U_error(suffix->pos, "field name (%s)not exist",
                            S_get_name(name));

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

static T_type T_trans_type(S_table tenv, A_type n)
{
    switch(n->kind) {
        case A_kind_type_name: {
            S_symbol name = n->u.name;
            T_type   name_ty;

            // check type name.
            name_ty = S_look(tenv, name);
            if(!name_ty)
                U_error(n->pos, "type(%s) not exist", S_get_name(name));

            return name_ty;
        }

        case A_kind_type_array: {
            S_symbol element = n->u.array;
            T_type   element_ty;

            // check element type name.
            element_ty = S_look(tenv, element);
            if(!element_ty)
                U_error(n->pos, "element type(%s) not exist",
                        S_get_name(element));

            return T_array(element_ty);
        }

        case A_kind_type_record: {
            A_para_list  record = n->u.record;
            T_field_list fields;

            A_para_list  p;
            T_field_list f;

            // make field list.
            for(p = record, fields = NULL; p; p = p->tail) {
                S_symbol name = p->head->name;
                S_symbol type = p->head->type;
                T_type   type_ty;
                T_field  field;

                type_ty = S_look(tenv, type);
                if (!type_ty)
                    U_error(n->pos, "field type(%s) not exist",
                            S_get_name(type));

                field = T_mk_field(name, type_ty);
                if (!fields) {
                    f = T_mk_field_list(field, NULL);
                    fields = f;
                } else {
                    f->tail = T_mk_field_list(field, NULL);
                    f = f->tail;
                }
            }

            return T_record(fields);
        }

        default:
            U_error(n->pos, "unkown type definition");
    }
}

/****************************************************************************
 * Public
 ****************************************************************************/

T_tyir T_trans(A_exp root) {
    S_table venv = S_mk_table();
    S_table tenv = S_mk_table();

    S_enter(tenv, S_mk_symbol("int"), T_int());
    S_enter(tenv, S_mk_symbol("string"), T_str());

    return T_trans_exp(venv, tenv, root);
}
