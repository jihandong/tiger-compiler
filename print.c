/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "ast.h"
#include "symbol.h"
#include "type.h"
#include "util.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

// print white spaces
#define WHITE(x) ({ int i; for (i = 0; i <= x; i++) fprintf(out, ".   "); })

static const char *str_op[] =
{
    "plus",
    "minus",
    "times",
    "divide",
    "eq",
    "neq",
    "lt",
    "le",
    "gt",
    "ge",
};

static const char *str_type[] =
{
    "nil",
    "int",
    "str",
    "void",
    "name",
    "func",
    "array",
    "record",
};

/****************************************************************************
 * Private: ast display
 ****************************************************************************/

static void A_pr_dec(FILE *out, A_dec n, int d);
static void A_pr_exp(FILE *out, A_exp n, int d);
static void A_pr_var(FILE *out, A_var n, int d);
static void A_pr_type(FILE *out, A_type n, int d);
static void A_pr_dec_list(FILE *out, A_dec_list n, int d);
static void A_pr_exp_list(FILE *out, A_exp_list n, int d);
static void A_pr_para(FILE *out, A_para n, int d);
static void A_pr_para_list(FILE *out, A_para_list n, int d);
static void A_pr_arg(FILE *out, A_arg n, int d);
static void A_pr_arg_list(FILE *out, A_arg_list n, int d);

static inline void A_pr_op(FILE *out, int oper)
{
    fprintf(out, "%s", str_op[oper]);
}

static void A_pr_dec(FILE *out, A_dec n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case A_kind_dec_var:
            fprintf(out, "dec_variable(\n");
            if (n->u.var.type) {
                WHITE(d + 1); fprintf(out, "type:%s\n",
                                      S_get_name(n->u.var.type));
            }
            A_pr_exp(out, n->u.var.init, d + 1);
            WHITE(d + 1); fprintf(out, "escape:%s\n",
                                  n->u.var.escape ? "true" : "false");
            break;

        case A_kind_dec_type:
            fprintf(out, "dec_type(\n");
            WHITE(d + 1); fprintf(out, "name:%s\n",
                                  S_get_name(n->u.type.name));
            A_pr_type(out, n->u.type.type, d + 1);
            break;

        case A_kind_dec_func:
            fprintf(out, "dec_function(\n");
            WHITE(d + 1); fprintf(out, "name:%s\n", S_get_name(n->u.func.name));
            A_pr_para_list(out, n->u.func.paras, d + 1);
            if (n->u.func.ret) {
                WHITE(d + 1); fprintf(out, "return:%s\n",
                                      S_get_name(n->u.func.ret));
            }
            A_pr_exp(out, n->u.func.body, d + 1);
            break;

        default:
            U_error(-1, "Unkown dec node");
    }
    WHITE(d); fprintf(out, ")\n");
}

static void A_pr_exp(FILE *out, A_exp n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case A_kind_exp_var:
            fprintf(out, "exp_variable(\n");
            A_pr_var(out, n->u.var, d + 1);
            break;

        case A_kind_exp_nil:
            fprintf(out, "exp_nil()\n");
            return;

        case A_kind_exp_int:
            fprintf(out, "exp_integer(%d)\n", n->u.int_);
            return;

        case A_kind_exp_str:
            fprintf(out, "exp_string(%s)\n", n->u.str_);
            return;

        case A_kind_exp_call:
            fprintf(out, "exp_call(\n");
            WHITE(d + 1); fprintf(out, "func:%s\n", S_get_name(n->u.call.func));
            A_pr_exp_list(out, n->u.call.args, d + 1);
            break;

        case A_kind_exp_op:
            fprintf(out, "exp_op(\n");
            WHITE(d + 1); A_pr_op(out, n->u.op.oper); fprintf(out, "\n");
            A_pr_exp(out, n->u.op.left, d + 1);
            A_pr_exp(out, n->u.op.right, d + 1);
            break;

        case A_kind_exp_array:
            fprintf(out, "exp_array(\n");
            WHITE(d + 1); fprintf(out, "array:%s\n",
                                  S_get_name(n->u.array.type));
            A_pr_exp(out, n->u.array.size, d + 1);
            A_pr_exp(out, n->u.array.init, d + 1);
            break;

        case A_kind_exp_record:
            fprintf(out, "exp_record(\n");
            A_pr_arg_list(out, n->u.record.args, d + 1);
            break;

        case A_kind_exp_seq:
            fprintf(out, "exp_sequence(\n");
            A_pr_exp_list(out, n->u.seq, d + 1);
            break;

        case A_kind_exp_assign:
            fprintf(out, "exp_assign(\n");
            A_pr_var(out, n->u.assign.var, d + 1);
            A_pr_exp(out, n->u.assign.exp, d + 1);
            break;

        case A_kind_exp_if:
            fprintf(out, "exp_if(\n");
            A_pr_exp(out, n->u.if_.cond, d + 1);
            A_pr_exp(out, n->u.if_.then, d + 1);
            if (n->u.if_.else_) {
                A_pr_exp(out, n->u.if_.else_, d + 1);
            }
            break;

        case A_kind_exp_while:
            fprintf(out, "exp_while(\n");
            A_pr_exp(out, n->u.while_.cond, d + 1);
            A_pr_exp(out, n->u.while_.body, d + 1);
            break;

        case A_kind_exp_for:
            fprintf(out, "exp_for(\n");
            WHITE(d + 1); fprintf(out, "var:%s\n", S_get_name(n->u.for_.var));
            A_pr_exp(out, n->u.for_.lo, d + 1);
            A_pr_exp(out, n->u.for_.hi, d + 1);
            A_pr_exp(out, n->u.for_.body, d + 1);
            WHITE(d + 1); fprintf(out, "escape:%s\n",
                                  n->u.for_.escape ? "true" : "false");
            break;

        case A_kind_exp_break:
            fprintf(out, "exp_break(\n");
            return;

        case A_kind_exp_let:
            fprintf(out, "exp_let(\n");
            A_pr_dec_list(out, n->u.let.decs, d + 1);
            A_pr_exp_list(out, n->u.let.body, d + 1);
            break;

        default:
            U_error(-1, "Unkown exp node");
    }
    WHITE(d); fprintf(out, ")\n");
}

static void A_pr_var(FILE *out, A_var n, int d)
{
    if (n) {
        WHITE(d);
        switch(n->kind) {
            case A_kind_var_base:
                fprintf(out, "var_base(\n");
                WHITE(d + 1); fprintf(out, "base:%s\n",
                                      S_get_name(n->u.base.name));
                A_pr_var(out, n->u.index.suffix, d + 1);
                return;

            case A_kind_var_index:
                fprintf(out, "var_array_index(\n");
                A_pr_exp(out, n->u.index.exp, d + 1);
                A_pr_var(out, n->u.index.suffix, d + 1);
                break;

            case A_kind_var_field:
                fprintf(out, "var_record_field(\n");
                WHITE(d + 1); fprintf(out, "field:%s\n",
                                      S_get_name(n->u.field.name));
                A_pr_var(out, n->u.index.suffix, d + 1);
                break;

            default:
                U_error(-1, "Unkown var node");
        }
        WHITE(d); fprintf(out, ")\n");
    }
}

static void A_pr_type(FILE *out, A_type n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case A_kind_type_name:
            fprintf(out, "type_name(%s)\n", S_get_name(n->u.name));
            return;

        case A_kind_type_array:
            fprintf(out, "type_array(%s)\n", S_get_name(n->u.array));
            return;

        case A_kind_type_record:
            fprintf(out, "type_record(\n");
            A_pr_para_list(out, n->u.record, d + 1);
            break;

        default:
            U_error(-1, "Unkown type node");
    }
    WHITE(d); fprintf(out, ")\n");
}

static void A_pr_dec_list(FILE *out, A_dec_list n, int d)
{
    WHITE(d); fprintf(out, "dec_list(");
    if (n) {
        fprintf(out, "\n");
        A_pr_dec(out, n->head, d + 1);
        A_pr_dec_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

static void A_pr_exp_list(FILE *out, A_exp_list n, int d)
{
    WHITE(d); fprintf(out, "exp_list(");
    if (n) {
        fprintf(out, "\n");
        A_pr_exp(out, n->head, d + 1);
        A_pr_exp_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

static void A_pr_para(FILE *out, A_para n, int d)
{
    WHITE(d); fprintf(out, "para(\n");
    WHITE(d + 1); fprintf(out, "var:%s\n", S_get_name(n->name));
    WHITE(d + 1); fprintf(out, "type:%s\n", S_get_name(n->type));
    WHITE(d + 1); fprintf(out, "escape:%s\n", n->escape ? "true" : "false");
    WHITE(d); fprintf(out, ")\n");
}

static void A_pr_para_list(FILE *out, A_para_list n, int d)
{
    WHITE(d); fprintf(out, "para_list(");
    if (n) {
        fprintf(out, "\n");
        A_pr_para(out, n->head, d + 1);
        A_pr_para_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

static void A_pr_arg(FILE *out, A_arg n, int d)
{
    WHITE(d); fprintf(out, "arg(\n");
    WHITE(d + 1); fprintf(out, "field:%s\n", S_get_name(n->name));
    A_pr_exp(out, n->exp, d + 1);
    WHITE(d); fprintf(out, ")\n");
}

static void A_pr_arg_list(FILE *out, A_arg_list n, int d)
{
    WHITE(d); fprintf(out, "arg_list(");
    if (n) {
        fprintf(out, "\n");
        A_pr_arg(out, n->head, d + 1);
        A_pr_arg_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

/****************************************************************************
 * Private: type display
 ****************************************************************************/

static void T_pr_type(FILE *out, T_type t);
static void T_pr_type_list(FILE *out, T_type_list t);
static void T_pr_field_list(FILE *out, T_field_list t);

static void T_pr_type(FILE *out, T_type t)
{
    if (!t)
        fprintf(out, "<none>");

    switch(t->kind) {
        case T_kind_nil:
        case T_kind_int:
        case T_kind_str:
        case T_kind_void:
            fprintf(out, "%s", str_type[t->kind]);
            break;

        case T_kind_name:
            fprintf(out, "`%s", S_get_name(t->u.name.symbol));
            break;

        case T_kind_func:
            T_pr_type_list(out, t->u.func.paras);
            fprintf(out, "->");
            T_pr_type(out, t->u.func.ret);
            break;

        case T_kind_array: 
            T_pr_type(out, t->u.array);
            fprintf(out, "[]");
            break;

        case T_kind_record:
            fprintf(out, "{");
            T_pr_field_list(out, t->u.record);
            fprintf(out, "}");
            break;

        default:
            U_error(-1, "unkown type");
    }
}

static void T_pr_type_list(FILE *out, T_type_list t)
{
    if (t) {
        T_pr_type(out, t->head);
        if (t->tail) {
            fprintf(out, ",");
            T_pr_type_list(out, t->tail);
        }
    }
}

static void T_pr_field_list(FILE *out, T_field_list t)
{
    if (t) {
        fprintf(out, "%s:", S_get_name(t->head->name));
        T_pr_type(out, t->head->type);
        if (t->tail) {
            fprintf(out, ",");
            T_pr_field_list(out, t->tail);
        }
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void A_print(FILE *out, A_exp root)
{
    A_pr_exp(out, root, 0);
}

void T_print(FILE *out, T_type type)
{
    T_pr_type(out, type);
}
