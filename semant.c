/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>
#include "env.h"
#include "semant.h"
#include "symbol.h"
#include "translate.h"
#include "type.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

#define printt(x,y) ({ printf("%s\t", x); TY_print(stdout, y); printf("\n"); })

typedef void *IR_ir;             /*< ir not implemented yet */

typedef struct SMT_tyir_ SMT_tyir;  /*< ir with type */

struct SMT_tyir_
{
    IR_ir    ir;
    TY_type  type;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static SMT_tyir SMT_mk_tyir(IR_ir ir, TY_type type)
{
    SMT_tyir e;

    e.ir    = ir;
    e.type  = type;

    return e;
}

/****************************************************************************
 * Trans (do type check now)
 ****************************************************************************/

/**
 * @brief Translate declarations.
 * can detect loop type definitions; support recursive definitions.
 * @param[in] level Static link level.
 * @param[in] venv  Value environment for variables and functions.
 * @param[in] tenv  Type environment for types.
 * @param[in] n     Astnode.
 */
static void SMT_trans_dec(TR_level level, SYM_table venv, SYM_table tenv,
                          AST_dec_list n);

/**
 * @brief Translate expressions.
 * support break validity.
 * @param[in] level Static link level.
 * @param[in] venv  Value environment for variables and functions.
 * @param[in] tenv  Type environment for types.
 * @param[in] n     Astnode.
 * @param[in] loop  Loop layer counter.
 * @return SMT_tyir Translated ir with type.
 */
static SMT_tyir SMT_trans_exp(TR_level level, SYM_table venv, SYM_table tenv,
                              AST_exp n, int loop);

/**
 * @brief Translate variables(lvalue).
 * @param[in] level Static link level.
 * @param[in] venv  Value environment for variables and functions.
 * @param[in] tenv  Type environment for types.
 * @param[in] n     Astnode.
 * @return SMT_tyir Translated ir with type.
 */
static SMT_tyir SMT_trans_var(TR_level level, SYM_table venv, SYM_table tenv,
                              AST_var n);

/**
 * @brief Translate types.
 * @param[in] level Static link level.
 * @param[in] tenv  Type environment for types.
 * @param[in] n     Astnode.
 * @return TY_type  Translated ir with type.
 * @note won't look into TY_name(name, ...) as it might not be defined yet.
 */
static TY_type SMT_trans_type(TR_level level, SYM_table tenv, AST_type n);

static void SMT_trans_dec(TR_level level, SYM_table venv, SYM_table tenv,
                          AST_dec_list n)
{
    AST_dec_list  vars   = NULL;
    AST_dec_list  types  = NULL;
    AST_dec_list  funcs  = NULL;
    TY_type_list  dummys = NULL;
    AST_dec_list  p, v, t, f;
    TY_type_list  d;

    //////////////////////////////////////////////////////////////////////////
    // Declarations Separation
    //////////////////////////////////////////////////////////////////////////

    // separate decs into 3 list: variables, types, functions.
    for (p = n; p; p = p->tail) {
        AST_dec dec = p->head;

        switch(dec->kind) {
            case AST_kind_dec_var: {
                // make variable declaratoin list
                if (!vars) {
                    v = AST_mk_dec_list(dec, NULL);
                    vars = v;
                } else {
                    v->tail = AST_mk_dec_list(dec, NULL);
                    v = v->tail;
                }
                break;
            }

            case AST_kind_dec_type: {
                SYM_symbol name  = dec->u.type.name;
                TY_type    dummy = TY_mk_name(name, NULL);

                // advertise dummy type
                SYM_enter(tenv, name, dummy);

                if (!dummys) {
                    d = TY_mk_type_list(dummy, NULL);
                    dummys = d;
                } else {
                    d->tail = TY_mk_type_list(dummy, NULL);
                    d = d->tail;
                }

                // make type declaratoin list
                if (!types) {
                    t = AST_mk_dec_list(dec, NULL);
                    types = t;
                } else {
                    t->tail = AST_mk_dec_list(dec, NULL);
                    t = t->tail;
                }
                break;
            }

            case AST_kind_dec_func: {
                // make function declaratoin list
                if (!funcs) {
                    f = AST_mk_dec_list(dec, NULL);
                    funcs = f;
                } else {
                    f->tail = AST_mk_dec_list(dec, NULL);
                    f = f->tail;
                }
                break;
            }

            default:
                UTL_error(dec->pos, "unkown declaration");
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Type Declarations
    //////////////////////////////////////////////////////////////////////////

    // translate type definitions
    for (t = types; t; t = t->tail) {
        AST_dec    dec  = t->head;
        SYM_symbol name = dec->u.type.name;
        AST_type   type = dec->u.type.type;
        TY_type    type_ty = SMT_trans_type(level, tenv, type);

        // cover dummy with raw definitions
        SYM_enter(tenv, name, type_ty);
    }

    // replace dummy type with translated definitions
    for (d = dummys; d; d = d->tail) {
        TY_type    dummy  = d->head;
        SYM_symbol symbol = dummy->u.name.symbol;
        SYM_symbol s      = symbol;

        while(1) {
            TY_type look = SYM_look(tenv, s);

            if (look->kind == TY_kind_name) {
                SYM_symbol lsymbol = look->u.name.symbol;
                TY_type   ltype   = look->u.name.type;

                // detect illegal loop definition
                if (lsymbol == symbol)
                    UTL_error(UTL_NOPOS, "LOOP");

                s = lsymbol;
            } else {
                dummy->u.name.type = look;
                break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Variable Declarations
    //////////////////////////////////////////////////////////////////////////

    // translate variable definitions
    for (v = vars; v; v = v->tail) {
        AST_dec    dec    = v->head;
        SYM_symbol name   = dec->u.var.name;
        SYM_symbol type   = dec->u.var.type;
        AST_exp    init   = dec->u.var.init;
        bool       escape = dec->u.var.escape;
        TY_type    type_ty;
        SMT_tyir   init_tyir;
        TR_access  access;
        ENV_entry  entry;

        // check variable init.
        init_tyir = SMT_trans_exp(level, venv, tenv, init, 0);
        if (type) {
            type_ty = SYM_look(tenv, type);
            if (!TY_match(init_tyir.type, type_ty)) {
                printt("type", type_ty);
                printt("init", init_tyir.type);
                UTL_error(dec->pos, "dec var(%s), type not match\n",
                        SYM_get_name(name));
            }
        }

        // variable allocation and keep entry.
        access = TR_alloc_local(level, escape);
        entry  = ENV_mk_entry_var(access, init_tyir.type);
        SYM_enter(venv, name, entry);
    }

    //////////////////////////////////////////////////////////////////////////
    // Function Declarations
    //////////////////////////////////////////////////////////////////////////

    // advertise function heads (paras -> ret)
    for (f = funcs; f; f = f->tail) {
        AST_dec       dec   = f->head;
        SYM_symbol    fname = dec->u.func.name;
        AST_para_list paras = dec->u.func.paras;
        SYM_symbol    ret   = dec->u.func.ret;
        AST_exp       body  = dec->u.func.body;

        UTL_bool_list para_escs;
        TY_type_list  para_tys;
        TY_type       ret_ty;
        SMT_tyir      body_tyir;
        TR_level      child;
        TMP_label     label;
        ENV_entry     entry;

        AST_para_list p;
        TY_type_list  t;
        UTL_bool_list b;

        // check return type.
        ret_ty = ret ? SYM_look(tenv, ret) : TY_void();
        if (!ret_ty) {
            UTL_error(dec->pos, "dec fuc(%s), ret(%s) not defined",
                    SYM_get_name(fname), SYM_get_name(ret));
        }

        // check parameter type.
        for (p = paras, para_tys = NULL, para_escs = NULL; p; p = p->tail) {
            SYM_symbol type   = p->head->type;
            bool       escape = p->head->escape;
            TY_type para_ty;

            para_ty = SYM_look(tenv, type);
            if (!para_ty) {
                UTL_error(dec->pos, "dec func(%s), para(%s) not defined",
                        SYM_get_name(fname), SYM_get_name(type));
            }

            // make parameter type list.
            if (!para_tys) {
                t = TY_mk_type_list(para_ty, NULL);
                para_tys = t;
            } else {
                t->tail = TY_mk_type_list(para_ty, NULL);
                t = t->tail;
            }

            // make parameter escape list.
            if (!para_escs) {
                b = UTL_mk_bool_list(escape, NULL);
                para_escs = b;
            } else {
                b->tail = UTL_mk_bool_list(escape, NULL);
                b = b->tail;
            }
        }

        // make child level and entry.
        label = TMP_mk_label();
        child = TR_mk_level(level, label, para_escs);
        entry = ENV_mk_entry_func(child, label, para_tys, ret_ty);
        SYM_enter(venv, fname, entry);
    }

    // translate function bodies
    for (f = funcs; f; f = f->tail) {
        AST_dec        dec   = f->head;
        SYM_symbol     fname = dec->u.func.name;
        AST_para_list  paras = dec->u.func.paras;
        AST_exp        body  = dec->u.func.body;

        ENV_entry      func     = SYM_look(venv, fname);
        TR_access_list para_acs = TR_get_paras(func->u.func.level);
        TY_type_list   para_tys = func->u.func.paras;
        SMT_tyir       body_tyir;
        ENV_entry      entry;

        AST_para_list  p;
        TY_type_list   t;
        TR_access_list a;

        SYM_begin(venv);

        for (p = paras, t = para_tys, a = para_acs; p && t && a;
             p = p->tail, t = t->tail, a = a->tail) {
            SYM_symbol name  = p->head->name;
            TY_type    type  = t->head;
            TR_access  acess = a->head;

            // parameter already alloced, make entry is ok.
            entry = ENV_mk_entry_var(acess, type);
            SYM_enter(venv, name, entry);
        }
        body_tyir = SMT_trans_exp(level, venv, tenv, body, 0); // do nothing

        SYM_end(venv);
    }
}

static SMT_tyir SMT_trans_exp(TR_level level, SYM_table venv, SYM_table tenv,
                              AST_exp n, int loop)
{
    switch(n->kind) {
        case AST_kind_exp_var:
            return SMT_trans_var(level, venv, tenv, n->u.var);

        case AST_kind_exp_nil:
            return SMT_mk_tyir(NULL, TY_nil());

        case AST_kind_exp_int:
            return SMT_mk_tyir(NULL, TY_int());

        case AST_kind_exp_str:
            return SMT_mk_tyir(NULL, TY_str());

        case AST_kind_exp_call: {
            SYM_symbol   func = n->u.call.func;
            AST_exp_list args = n->u.call.args;

            TY_type      func_ty, ret_ty;
            TY_type_list para_tys;

            // check funtion.
            func_ty = SYM_look(venv, func);
            if (!func_ty) {
                UTL_error(n->pos, "exp call, func(%s) not defined",
                        SYM_get_name(func));
            }

            ret_ty = func_ty->u.func.ret;

            // check parameter type.
            para_tys = func_ty->u.func.paras;
            for (; args && para_tys; args     = args->tail,
                                    para_tys = para_tys->tail) {
                AST_exp  arg    = args->head;
                TY_type para_ty = para_tys->head;
                SMT_tyir arg_tyir;

                arg_tyir = SMT_trans_exp(level, venv, tenv, arg, loop);
                if (!TY_match(para_ty, arg_tyir.type)) {
                    printt("para", para_ty);
                    printt("arg", arg_tyir.type);
                    UTL_error(n->pos,
                            "exp call, func(%s), para and arg not match",
                            SYM_get_name(func));
                }
            }

            return SMT_mk_tyir(NULL, ret_ty);
        }

        case AST_kind_exp_op: {
            AST_exp  left  = n->u.op.left;
            AST_exp  right = n->u.op.right;
            SMT_tyir left_tyir, right_tyir;

            // check left operand
            left_tyir = SMT_trans_exp(level, venv, tenv, left, loop);
            if (TY_get_kind(left_tyir.type) != TY_kind_int) {
                printt("left", left_tyir.type);
                UTL_error(left->pos, "exp op, left is not integer");
            }

            // check right operand
            right_tyir = SMT_trans_exp(level, venv, tenv, right, loop);
            if (TY_get_kind(right_tyir.type) != TY_kind_int) {
                printt("right", right_tyir.type);
                UTL_error(right->pos, "exp op, right is not integer");
            }

            return SMT_mk_tyir(NULL, TY_int());
        }

        case AST_kind_exp_array: {
            SYM_symbol array = n->u.array.type;
            AST_exp    size  = n->u.array.size;
            AST_exp    init  = n->u.array.init;
            TY_type    array_ty;
            SMT_tyir   size_tyir, init_tyir;

            // check array type.
            array_ty = SYM_look(tenv, array);
            if (!array_ty)
                UTL_error(n->pos, "exp array(%s), not defined",
                        SYM_get_name(array));

            if (TY_get_kind(array_ty) != TY_kind_array) {
                printt("array", array_ty);
                UTL_error(n->pos, "exp array(%s), is not array",
                        SYM_get_name(array));
            }

            // check array size.
            size_tyir = SMT_trans_exp(level, venv, tenv, size, loop);
            if (TY_get_kind(size_tyir.type) != TY_kind_int) {
                printt("array", size_tyir.type);
                UTL_error(size->pos, "exp array(%s), size is not integer",
                        SYM_get_name(array));
            }

            // check array init.
            init_tyir = SMT_trans_exp(level, venv, tenv, init, loop);
            if (init_tyir.type != array_ty->u.array) {
                printt("element", array_ty->u.array);
                printt("init", init_tyir.type);
                UTL_error(init->pos,
                        "exp array(%s), element and init not match",
                        SYM_get_name(array));
            }

            return SMT_mk_tyir(NULL, array_ty);
        }

        case AST_kind_exp_record: {
            SYM_symbol    record = n->u.record.type;
            AST_arg_list  args   = n->u.record.args;
            TY_type       record_ty;
            TY_field_list fields;

            // check record type.
            record_ty = SYM_look(tenv, record);
            if (!record_ty) {
                UTL_error(n->pos, "exp record(%s), not defined",
                        SYM_get_name(record));
            }
            fields = record_ty->u.record;

            // check fields type.
            for (; args && fields; args = args->tail, fields = fields->tail) {
                SYM_symbol name1 = args->head->name;
                AST_exp    exp   = args->head->exp;
                SYM_symbol name2 = fields->head->name;
                TY_type    type  = fields->head->type;
                SMT_tyir   exp_tyir;

                if (name1 != name2) {
                    UTL_error(exp->pos, "exp record(%s), name not match"
                            "give %s, nedd %s", SYM_get_name(record),
                            SYM_get_name(name1), SYM_get_name(name2));
                }

                exp_tyir = SMT_trans_exp(level ,venv, tenv, exp, loop);
                if (!TY_match(type, exp_tyir.type)) {
                    printt("give", exp_tyir.type);
                    printt("need", type);
                    UTL_error(exp->pos, "exp record(%s), type not match",
                            SYM_get_name(record));
                }
            }

            return SMT_mk_tyir(NULL, record_ty);
        }

        case AST_kind_exp_seq: {
            AST_exp_list seq = n->u.seq;
            SMT_tyir     exp_tyir;
            AST_exp_list s;

            for (s = seq; s; s = s->tail)
                exp_tyir = SMT_trans_exp(level, venv, tenv, s->head, loop);

            return SMT_mk_tyir(NULL, seq ? exp_tyir.type : TY_void());
        }

        case AST_kind_exp_assign: {
            AST_var  var = n->u.assign.var;
            AST_exp  exp = n->u.assign.exp;
            SMT_tyir var_tyir, exp_tyir;

            // check type match.
            var_tyir = SMT_trans_var(level, venv, tenv, var);
            exp_tyir = SMT_trans_exp(level, venv, tenv, exp, loop);
            if (!TY_match(var_tyir.type, exp_tyir.type)) {
                printt("var", var_tyir.type);
                printt("exp", exp_tyir.type);
                UTL_error(n->pos, "exp assign, type not match");
            }

            return SMT_mk_tyir(NULL, TY_void());
        }

        case AST_kind_exp_if: {
            AST_exp  cond  = n->u.if_.cond;
            AST_exp  then  = n->u.if_.then;
            AST_exp  else_ = n->u.if_.else_;
            SMT_tyir cond_tyir, then_tyir, else_tyir;

            // check condition type.
            cond_tyir = SMT_trans_exp(level, venv, tenv, cond, loop);
            if (TY_get_kind(cond_tyir.type) != TY_kind_int) {
                printt("cond", cond_tyir.type);
                UTL_error(cond->pos, "exp if, cond is not integer");
            }

            // if-then
            then_tyir = SMT_trans_exp(level, venv, tenv, then, loop);
            if (!else_)
                return SMT_mk_tyir(NULL, TY_void());

            // if-then-else, check branches type.
            else_tyir = SMT_trans_exp(level, venv, tenv, else_, loop);
            if (then_tyir.type != else_tyir.type) {
                printt("then", then_tyir.type);
                printt("else", else_tyir.type);
                UTL_error(n->pos, "exp if, branches type not match");
            }

            return SMT_mk_tyir(NULL, then_tyir.type);
        }

        case AST_kind_exp_while: {
            AST_exp  cond = n->u.while_.cond;
            AST_exp  body = n->u.while_.body;
            SMT_tyir cond_tyir, body_tyir;

            // check condition type.
            cond_tyir = SMT_trans_exp(level, venv, tenv, cond, loop);
            if (TY_get_kind(cond_tyir.type) != TY_kind_int) {
                printt("cond", cond_tyir.type);
                UTL_error(cond->pos, "exp while, cond is not integer");
            }

            SYM_begin(venv);
            body_tyir = SMT_trans_exp(level, venv, tenv, body, loop + 1);
            SYM_end(venv);

            return SMT_mk_tyir(NULL, TY_void());
        }

        case AST_kind_exp_for: {
            SYM_symbol var  = n->u.for_.var;
            AST_exp    lo   = n->u.for_.lo;
            AST_exp    hi   = n->u.for_.hi;
            AST_exp    body = n->u.for_.body;
            SMT_tyir   lo_tyir, hi_tyir, body_tyir;

            // check lowest exp type.
            lo_tyir = SMT_trans_exp(level, venv, tenv, lo, loop);
            if (TY_get_kind(lo_tyir.type) != TY_kind_int) {
                printt("low", lo_tyir.type);
                UTL_error(lo->pos, "exp for, low is not integer");
            }

            // check highest exp type.
            hi_tyir = SMT_trans_exp(level, venv, tenv, hi, loop);
            if (TY_get_kind(hi_tyir.type) != TY_kind_int) {
                printt("high", hi_tyir.type);
                UTL_error(hi->pos, "exp for, high is not integer");
            }

            SYM_begin(venv);
            SYM_enter(venv, var, TY_int());

            body_tyir = SMT_trans_exp(level, venv, tenv, body, loop + 1);

            SYM_end(venv);

            return SMT_mk_tyir(NULL, body_tyir.type);
        }

        case AST_kind_exp_break:
            if (loop <= 0)
                UTL_error(n->pos, "exp break, not in loop");

            return SMT_mk_tyir(NULL, TY_void());

        case AST_kind_exp_let: {
            AST_dec_list decs = n->u.let.decs;
            AST_exp_list body = n->u.let.body;
            SMT_tyir     body_tyir;
            AST_exp_list b;

            SYM_begin(venv);
            SYM_begin(tenv);

            SMT_trans_dec(level, venv, tenv, decs);
            for (b = body; b; b = b->tail)
                body_tyir = SMT_trans_exp(level, venv, tenv, b->head, loop);

            SYM_end(tenv);
            SYM_end(venv);

            return SMT_mk_tyir(NULL, body? body_tyir.type : TY_void());
        }

        default:
            UTL_error(n->pos, "unkown expression");
    }
}

static SMT_tyir SMT_trans_var(TR_level level, SYM_table venv, SYM_table tenv,
                              AST_var n)
{
    SYM_symbol base;
    AST_var    suffix;
    TY_type    base_ty;
    SMT_tyir   var_tyir;
    AST_var    p;
    TY_type    t;

    if (n->kind != AST_kind_var_base)
        UTL_error(n->pos, "lvalue, emtpy");

    // check symbol.
    base    = n->u.base.name;
    suffix  = n->u.base.suffix;
    base_ty = SYM_look(venv, base);
    if (!base_ty)
        UTL_error(n->pos, "lvalue, base(%s) not defined", SYM_get_name(base));

    for (p = suffix, t = base_ty; p;) {
        switch(suffix->kind) {
            case AST_kind_var_base:
                UTL_error(suffix->pos, "lvalue, two bases?");

            case AST_kind_var_index: {
                AST_exp  exp      = p->u.index.exp;
                AST_var  suffix   = p->u.index.suffix;
                SMT_tyir exp_tyir = SMT_trans_exp(level, venv, tenv, exp, 0);

                // check array type.
                if (TY_get_kind(t) != TY_kind_array) {
                    printt("array", t);
                    UTL_error(suffix->pos, "lvalue, base is not array");
                }

                // check index type.
                if (TY_get_kind(exp_tyir.type) != TY_kind_int) {
                    printt("index", exp_tyir.type);
                    UTL_error(suffix->pos, "lvalue, index is not integer");
                }

                // update.
                p = suffix;
                t = t->u.array;
                break;
            }

            case AST_kind_var_field: {
                SYM_symbol    name   = p->u.field.name;
                AST_var       suffix = p->u.index.suffix;
                TY_field_list fields;
                TY_field      field;

                // check record type.
                if (TY_get_kind(t) != TY_kind_record) {
                    printt("record", t);
                    UTL_error(suffix->pos, "lvalue, base is not record");
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
                    UTL_error(suffix->pos, "lvalue, field(%s) not defined",
                            SYM_get_name(name));

                // update.
                p = suffix;
                t = field->type;
                break;
            }

            default :
                UTL_error(n->pos, "unkown lvalue");
        }
    }

    return SMT_mk_tyir(NULL, t);
}

static TY_type SMT_trans_type(TR_level level, SYM_table tenv, AST_type n)
{
    switch(n->kind) {
        case AST_kind_type_name: {
            SYM_symbol name = n->u.name;
            TY_type    name_ty;

            // check type name.
            name_ty = SYM_look(tenv, name);
            if(!name_ty) {
                UTL_error(n->pos, "type name(%s), not defined",
                        SYM_get_name(name));
            }

            return name_ty;
        }

        case AST_kind_type_array: {
            SYM_symbol element = n->u.array;
            TY_type    element_ty;

            // check element type name.
            element_ty = SYM_look(tenv, element);
            if(!element_ty) {
                UTL_error(n->pos, "type array, element(%s) not defined",
                        SYM_get_name(element));
            }

            return TY_mk_array(element_ty);
        }

        case AST_kind_type_record: {
            AST_para_list record = n->u.record;
            TY_field_list fields;

            AST_para_list p;
            TY_field_list f;

            // make field list.
            for (p = record, fields = NULL; p; p = p->tail) {
                SYM_symbol name = p->head->name;
                SYM_symbol type = p->head->type;
                TY_type    type_ty;
                TY_field   field;

                type_ty = SYM_look(tenv, type);
                if (!type_ty) {
                    UTL_error(n->pos, "type record, field(%s) not defined",
                            SYM_get_name(type));
                }

                field = TY_mk_field(name, type_ty);
                if (!fields) {
                    f = TY_mk_field_list(field, NULL);
                    fields = f;
                } else {
                    f->tail = TY_mk_field_list(field, NULL);
                    f = f->tail;
                }
            }

            return TY_mk_record(fields);
        }

        default:
            UTL_error(n->pos, "unkown type definition");
    }
}

void SMT_trans(AST_exp root)
{
    SYM_table venv = ENV_base_venv();
    SYM_table tenv = ENV_base_tenv();

    SMT_trans_exp(TR_root_level(), venv, tenv, root, 0);
}
