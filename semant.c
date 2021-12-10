/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>
#include "semant.h"
#include "symbol.h"
#include "type.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define printt(x,y) ({ printf("%s\t", x); T_print(stdout, y); printf("\n"); })

typedef void *I_ir;             /*< ir not implemented yet */
typedef struct T_tyir_ T_tyir;  /*< ir with type */

struct T_tyir_ {
    I_ir    ir;
    T_type  type;
};

/**
 * @brief Translate declarations.
 * can detect loop type definitions; support recursive definitions.
 * @param[in] venv  value environment for variables and functions.
 * @param[in] tenv  type environment for types.
 * @param[in] n     astnode.
 */
static void T_trans_dec(S_table venv, S_table tenv, A_dec_list n);
/**
 * @brief Translate expressions.
 * support break validity.
 * @param[in] venv  value environment for variables and functions.
 * @param[in] tenv  type environment for types.
 * @param[in] n     astnode.
 * @param[in] loop  loop layer counter.
 * @return T_tyir   translated ir with type.
 */
static T_tyir T_trans_exp(S_table venv, S_table tenv, A_exp n, int loop);
/**
 * @brief Translate variables(lvalue).
 * @param[in] venv  value environment for variables and functions.
 * @param[in] tenv  type environment for types.
 * @param[in] n     astnode.
 * @return T_tyir   translated ir with type.
 */
static T_tyir T_trans_var(S_table venv, S_table tenv, A_var n);
/**
 * @brief Translate types.
 * @param[in] tenv  type environment for types.
 * @param[in] n     astnode.
 * @return T_type   translated ir with type.
 * @note won't look into T_name(name, ...) as it might not be defined yet.
 */
static T_type T_trans_type(S_table tenv, A_type n);

/****************************************************************************
 * Private
 ****************************************************************************/

static inline T_tyir T_mk_tyir(I_ir ir, T_type type)
{
    T_tyir e;

    e.ir    = ir;
    e.type  = type;

    return e;
}

static void T_trans_dec(S_table venv, S_table tenv, A_dec_list n)
{
    A_dec_list  vars;
    A_dec_list  types;
    A_dec_list  funcs;
    T_type_list dummys;
    A_dec_list  p, v, t, f;
    T_type_list d;

    //////////////////////////////////////////////////////////////////////////
    // Declarations Separation
    //////////////////////////////////////////////////////////////////////////

    // separate decs into 3 list: variables, types, functions.
    for (p = n, vars = NULL, types = NULL, funcs = NULL; p; p = p->tail) {
        A_dec dec = p->head;

        switch(dec->kind) {
            case A_kind_dec_var: {
                // make variable declaratoin list
                if (!vars) {
                    v = A_mk_dec_list(dec, NULL);
                    vars = v;
                } else {
                    v->tail = A_mk_dec_list(dec, NULL);
                    v = v->tail;
                }
                break;
            }

            case A_kind_dec_type: {
                S_symbol name  = dec->u.type.name;
                T_type   dummy = T_mk_name(name, NULL);

                // advertise dummy type
                S_enter(tenv, name, dummy);

                if (!dummys) {
                    d = T_mk_type_list(dummy, NULL);
                    dummys = d;
                } else {
                    d->tail = T_mk_type_list(dummy, NULL);
                    d = d->tail;
                }

                // make type declaratoin list
                if (!types) {
                    t = A_mk_dec_list(dec, NULL);
                    types = t;
                } else {
                    t->tail = A_mk_dec_list(dec, NULL);
                    t = t->tail;
                }
                break;
            }

            case A_kind_dec_func: {
                // make function declaratoin list
                if (!funcs) {
                    f = A_mk_dec_list(dec, NULL);
                    funcs = f;
                } else {
                    f->tail = A_mk_dec_list(dec, NULL);
                    f = f->tail;
                }
                break;
            }

            default:
                U_error(dec->pos, "unkown declaration");
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Type Declarations
    //////////////////////////////////////////////////////////////////////////

    // translate type definitions
    for (t = types; t; t = t->tail) {
        A_dec    dec  = t->head;
        S_symbol name = dec->u.type.name;
        A_type   type = dec->u.type.type;
        T_type   type_ty = T_trans_type(tenv, type);

        // cover dummy with raw definitions
        S_enter(tenv, name, type_ty);
    }

    // replace dummy type with translated definitions
    for (d = dummys; d; d = d->tail) {
        T_type   dummy  = d->head;
        S_symbol symbol = dummy->u.name.symbol;
        S_symbol s      = symbol;

        while(1) {
            T_type look = S_look(tenv, s);

            if (look->kind == T_kind_name) {
                S_symbol lsymbol = look->u.name.symbol;
                T_type   ltype   = look->u.name.type;

                // detect illegal loop definition
                if (lsymbol == symbol)
                    U_error(-1, "LOOP");

                s = lsymbol;
            } else {
                dummy->u.name.type = look;
                break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Vatiable Declarations
    //////////////////////////////////////////////////////////////////////////

    // translate variable definitions
    for (v = vars; v; v = v->tail) {
        A_dec    dec  = v->head;
        S_symbol name = dec->u.var.name;
        S_symbol type = dec->u.var.type;
        A_exp    init = dec->u.var.init;
        T_type   type_ty;
        T_tyir   init_tyir;

        // check variable init.
        init_tyir = T_trans_exp(venv, tenv, init, 0);
        if (type) {
            type_ty = S_look(tenv, type);
            if (init_tyir.type != type_ty) {
                printt("type", type_ty);
                printt("init", init_tyir.type);
                U_error(dec->pos, "dec var(%s), type not match\n",
                        S_get_name(name));
            }
        }

        S_enter(venv, name, init_tyir.type);
    }

    //////////////////////////////////////////////////////////////////////////
    // Function Declarations
    //////////////////////////////////////////////////////////////////////////

    // advertise function heads (paras -> ret)
    for (f = funcs; f; f = f->tail) {
        A_dec       dec   = f->head;
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
        ret_ty = ret ? S_look(tenv, ret) : T_void();
        if (!ret_ty) {
            U_error(dec->pos, "dec fuc(%s), ret(%s) not defined",
                    S_get_name(fname), S_get_name(ret));
        }

        // check parameter type.
        for (p = paras, para_tys = NULL; p; p = p->tail) {
            S_symbol type = p->head->type;
            T_type para_ty;

            para_ty = S_look(tenv, type);
            if (!para_ty) {
                U_error(dec->pos, "dec func(%s), para(%s) not defined",
                        S_get_name(fname), S_get_name(type));
            }

            // make parameter list.
            if (!para_tys) {
                t = T_mk_type_list(para_ty, NULL);
                para_tys = t;
            } else {
                t->tail = T_mk_type_list(para_ty, NULL);
                t = t->tail;
            }
        }

        S_enter(venv, fname, T_mk_func(ret_ty, para_tys));
    }

    // translate function bodies
    for (f = funcs; f; f = f->tail) {
        A_dec       dec   = f->head;
        S_symbol    fname = dec->u.func.name;
        A_para_list paras = dec->u.func.paras;
        A_exp       body  = dec->u.func.body;

        T_type      func     = S_look(venv, fname);
        T_type_list para_tys = func->u.func.paras;
        T_tyir      body_tyir;

        A_para_list p;
        T_type_list t;

        S_begin(venv, S_get_name(fname));

        for (p = paras, t = para_tys; p && t; p = p->tail, t = t->tail) {
            S_symbol name = p->head->name;
            T_type   type = t->head;

            S_enter(venv, name, type);
        }
        body_tyir = T_trans_exp(venv, tenv, body, 0); // do nothing

        S_end(venv);
    }
}

static T_tyir T_trans_exp(S_table venv, S_table tenv, A_exp n, int loop)
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
            if (!func_ty) {
                U_error(n->pos, "exp call, func(%s) not defined",
                        S_get_name(func));
            }

            ret_ty = func_ty->u.func.ret;

            // check parameter type.
            para_tys = func_ty->u.func.paras;
            for (; args && para_tys; args     = args->tail,
                                    para_tys = para_tys->tail) {
                A_exp  arg     = args->head;
                T_type para_ty = para_tys->head;
                T_tyir arg_tyir;

                arg_tyir = T_trans_exp(venv, tenv, arg, loop);
                if (arg_tyir.type != para_ty) {
                    printt("para", para_ty);
                    printt("arg", arg_tyir.type);
                    U_error(n->pos,
                            "exp call, func(%s), para and arg not match",
                            S_get_name(func));
                }
            }

            return T_mk_tyir(NULL, ret_ty);
        }

        case A_kind_exp_op: {
            A_exp  left  = n->u.op.left;
            A_exp  right = n->u.op.right;
            T_tyir left_tyir, right_tyir;

            // check left operand
            left_tyir = T_trans_exp(venv, tenv, left, loop);
            if (T_get_kind(left_tyir.type) != T_kind_int) {
                printt("left", left_tyir.type);
                U_error(left->pos, "exp op, left is not integer");
            }

            // check right operand
            right_tyir = T_trans_exp(venv, tenv, right, loop);
            if (T_get_kind(right_tyir.type) != T_kind_int) {
                printt("right", right_tyir.type);
                U_error(right->pos, "exp op, right is not integer");
            }

            return T_mk_tyir(NULL, T_int());
        }

        case A_kind_exp_array: {
            S_symbol array = n->u.array.type;
            A_exp    size  = n->u.array.size;
            A_exp    init  = n->u.array.init;
            T_type   array_ty;
            T_tyir   size_tyir, init_tyir;

            // check array type.
            array_ty = S_look(tenv, array);
            if (!array_ty)
                U_error(n->pos, "exp array(%s), not defined",
                        S_get_name(array));

            if (T_get_kind(array_ty) != T_kind_array) {
                printt("array", array_ty);
                U_error(n->pos, "exp array(%s), is not array",
                        S_get_name(array));
            }

            // check array size.
            size_tyir = T_trans_exp(venv, tenv, size, loop);
            if (T_get_kind(size_tyir.type) != T_kind_int) {
                printt("array", size_tyir.type);
                U_error(size->pos, "exp array(%s), size is not integer",
                        S_get_name(array));
            }

            // check array init.
            init_tyir = T_trans_exp(venv, tenv, init, loop);
            if (init_tyir.type != array_ty->u.array) {
                printt("element", array_ty->u.array);
                printt("init", init_tyir.type);
                U_error(init->pos,
                        "exp array(%s), element and init not match",
                        S_get_name(array));
            }

            return T_mk_tyir(NULL, array_ty);
        }

        case A_kind_exp_record: {
            S_symbol     record = n->u.record.type;
            A_arg_list   args   = n->u.record.args;
            T_type       record_ty;
            T_field_list fields;

            // check record type.
            record_ty = S_look(tenv, record);
            if (!record_ty) {
                U_error(n->pos, "exp record(%s), not defined",
                        S_get_name(record));
            }
            fields = record_ty->u.record;

            // check fields type.
            for (; args && fields; args = args->tail, fields = fields->tail) {
                S_symbol name1 = args->head->name;
                A_exp    exp   = args->head->exp;
                S_symbol name2 = fields->head->name;
                T_type   type  = fields->head->type;
                T_tyir   exp_tyir;

                if (name1 != name2) {
                    U_error(exp->pos, "exp record(%s), name not match"
                            "give %s, nedd %s", S_get_name(record),
                            S_get_name(name1), S_get_name(name2));
                }

                exp_tyir = T_trans_exp(venv, tenv, exp, loop);
                if (exp_tyir.type != type) {
                    printt("give", exp_tyir.type);
                    printt("need", type);
                    U_error(exp->pos, "exp record(%s), type not match",
                            S_get_name(record));
                }
            }

            return T_mk_tyir(NULL, record_ty);
        }

        case A_kind_exp_seq: {
            A_exp_list seq = n->u.seq;
            T_tyir     exp_tyir;
            A_exp_list s;

            for (s = seq; s; s = s->tail)
                exp_tyir = T_trans_exp(venv, tenv, s->head, loop);

            return T_mk_tyir(NULL, seq ? exp_tyir.type : T_void());
        }

        case A_kind_exp_assign: {
            A_var  var = n->u.assign.var;
            A_exp  exp = n->u.assign.exp;
            T_tyir var_tyir, exp_tyir;

            // check type match.
            var_tyir = T_trans_var(venv, tenv, var);
            exp_tyir = T_trans_exp(venv, tenv, exp, loop);
            if (var_tyir.type != exp_tyir.type) {
                printt("var", var_tyir.type);
                printt("exp", exp_tyir.type);
                U_error(n->pos, "exp assign, type not match");
            }

            return T_mk_tyir(NULL, T_void());
        }

        case A_kind_exp_if: {
            A_exp  cond  = n->u.if_.cond;
            A_exp  then  = n->u.if_.then;
            A_exp  else_ = n->u.if_.else_;
            T_tyir cond_tyir, then_tyir, else_tyir;

            // check condition type.
            cond_tyir = T_trans_exp(venv, tenv, cond, loop);
            if (T_get_kind(cond_tyir.type) != T_kind_int) {
                printt("cond", cond_tyir.type);
                U_error(cond->pos, "exp if, cond is not integer");
            }

            // if-then
            then_tyir = T_trans_exp(venv, tenv, then, loop);
            if (!else_)
                return T_mk_tyir(NULL, T_void());

            // if-then-else, check branches type.
            else_tyir = T_trans_exp(venv, tenv, else_, loop);
            if (then_tyir.type != else_tyir.type) {
                printt("then", then_tyir.type);
                printt("else", else_tyir.type);
                U_error(n->pos, "exp if, branches type not match");
            }

            return T_mk_tyir(NULL, then_tyir.type);
        }

        case A_kind_exp_while: {
            A_exp  cond = n->u.while_.cond;
            A_exp  body = n->u.while_.body;
            T_tyir cond_tyir, body_tyir;

            // check condition type.
            cond_tyir = T_trans_exp(venv, tenv, cond, loop);
            if (T_get_kind(cond_tyir.type) != T_kind_int) {
                printt("cond", cond_tyir.type);
                U_error(cond->pos, "exp while, cond is not integer");
            }

            S_begin(venv, "while");
            body_tyir = T_trans_exp(venv, tenv, body, loop + 1);
            S_end(venv);

            return T_mk_tyir(NULL, T_void());
        }

        case A_kind_exp_for: {
            S_symbol var  = n->u.for_.var;
            A_exp    lo   = n->u.for_.lo;
            A_exp    hi   = n->u.for_.hi;
            A_exp    body = n->u.for_.body;
            T_tyir   lo_tyir, hi_tyir, body_tyir;

            // check lowest exp type.
            lo_tyir = T_trans_exp(venv, tenv, lo, loop);
            if (T_get_kind(lo_tyir.type) != T_kind_int) {
                printt("low", lo_tyir.type);
                U_error(lo->pos, "exp for, low is not integer");
            }

            // check highest exp type.
            hi_tyir = T_trans_exp(venv, tenv, hi, loop);
            if (T_get_kind(hi_tyir.type) != T_kind_int) {
                printt("high", hi_tyir.type);
                U_error(hi->pos, "exp for, high is not integer");
            }

            S_begin(venv, "for");
            S_enter(venv, var, T_int());

            body_tyir = T_trans_exp(venv, tenv, body, loop + 1);

            S_end(venv);

            return T_mk_tyir(NULL, body_tyir.type);
        }

        case A_kind_exp_break:
            if (loop <= 0)
                U_error(n->pos, "exp break, not in loop");

            return T_mk_tyir(NULL, T_void());

        case A_kind_exp_let: {
            A_dec_list decs = n->u.let.decs;
            A_exp_list body = n->u.let.body;
            T_tyir     body_tyir;
            A_exp_list b;

            S_begin(venv, "let");
            S_begin(tenv, "let");

            T_trans_dec(venv, tenv, decs);
            for (b = body; b; b = b->tail)
                body_tyir = T_trans_exp(venv, tenv, b->head, loop);

            S_end(tenv);
            S_end(venv);

            return T_mk_tyir(NULL, body? body_tyir.type : T_void());
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

    if (n->kind != A_kind_var_base)
        U_error(n->pos, "lvalue, emtpy");

    // check symbol.
    base    = n->u.base.name;
    suffix  = n->u.base.suffix;
    base_ty = S_look(venv, base);
    if (!base_ty)
        U_error(n->pos, "lvalue, base(%s) not defined", S_get_name(base));

    for (p = suffix, t = base_ty; p;) {
        switch(suffix->kind) {
            case A_kind_var_base:
                U_error(suffix->pos, "lvalue, two bases?");

            case A_kind_var_index: {
                A_exp  exp      = p->u.index.exp;
                A_var  suffix   = p->u.index.suffix;
                T_tyir exp_tyir = T_trans_exp(venv, tenv, exp, 0);

                // check array type.
                if (T_get_kind(t) != T_kind_array) {
                    printt("array", t);
                    U_error(suffix->pos, "lvalue, base is not array");
                }

                // check index type.
                if (T_get_kind(exp_tyir.type) != T_kind_int) {
                    printt("index", exp_tyir.type);
                    U_error(suffix->pos, "lvalue, index is not integer");
                }

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
                if (T_get_kind(t) != T_kind_record) {
                    printt("record", t);
                    U_error(suffix->pos, "lvalue, base is not record");
                }

                // check field name.
                fields = t->u.record;
                for (field = NULL; fields; fields = fields->tail) {
                    if (fields->head->name == name) {
                        field = fields->head;
                        break;
                    }
                }
                if (!field)
                    U_error(suffix->pos, "lvalue, field(%s) not defined",
                            S_get_name(name));

                // update.
                p = suffix;
                t = field->type;
                break;
            }

            default :
                U_error(n->pos, "unkown lvalue");
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
            if(!name_ty) {
                U_error(n->pos, "type name(%s), not defined",
                        S_get_name(name));
            }

            return name_ty;
        }

        case A_kind_type_array: {
            S_symbol element = n->u.array;
            T_type   element_ty;

            // check element type name.
            element_ty = S_look(tenv, element);
            if(!element_ty) {
                U_error(n->pos, "type array, element(%s) not defined",
                        S_get_name(element));
            }

            return T_mk_array(element_ty);
        }

        case A_kind_type_record: {
            A_para_list  record = n->u.record;
            T_field_list fields;

            A_para_list  p;
            T_field_list f;

            // make field list.
            for (p = record, fields = NULL; p; p = p->tail) {
                S_symbol name = p->head->name;
                S_symbol type = p->head->type;
                T_type   type_ty;
                T_field  field;

                type_ty = S_look(tenv, type);
                if (!type_ty) {
                    U_error(n->pos, "type record, field(%s) not defined",
                            S_get_name(type));
                }

                field = T_mk_field(name, type_ty);
                if (!fields) {
                    f = T_mk_field_list(field, NULL);
                    fields = f;
                } else {
                    f->tail = T_mk_field_list(field, NULL);
                    f = f->tail;
                }
            }

            return T_mk_record(fields);
        }

        default:
            U_error(n->pos, "unkown type definition");
    }
}

/****************************************************************************
 * Public
 ****************************************************************************/

void T_trans(A_exp root) {
    S_table venv = S_mk_table();
    S_table tenv = S_mk_table();

    S_enter(tenv, S_mk_symbol("int"), T_int());
    S_enter(tenv, S_mk_symbol("string"), T_str());

    T_trans_exp(venv, tenv, root, 0);
}
