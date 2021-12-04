/********************************************************************************
 * Include Files
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol.h"
#include "util.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

// print white spaces
#define WHITE(x) ({ int i; for(i = 0; i <= x; i++) fprintf(out, ".   "); })

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

static void Ap_dec(FILE *out, A_dec n, int d);
static void Ap_exp(FILE *out, A_exp n, int d);
static void Ap_var(FILE *out, A_var n, int d);
static void Ap_type(FILE *out, A_type n, int d);
static void Ap_dec_list(FILE *out, A_dec_list n, int d);
static void Ap_exp_list(FILE *out, A_exp_list n, int d);
static void Ap_para(FILE *out, A_para n, int d);
static void Ap_para_list(FILE *out, A_para_list n, int d);
static void Ap_arg(FILE *out, A_arg n, int d);
static void Ap_arg_list(FILE *out, A_arg_list n, int d);

/********************************************************************************
 * Private Functions
 ********************************************************************************/

static inline void Ap_op(FILE *out, int oper)
{
    fprintf(out, "%s", str_op[oper]);
}

static void Ap_dec(FILE *out, A_dec n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case A_kind_dec_var:
            fprintf(out, "dec_variable(\n");
            if (n->u.var.type) {
                WHITE(d + 1); fprintf(out, "type:%s\n",
                                      S_get_name(n->u.var.type));
            }
            Ap_exp(out, n->u.var.init, d + 1);
            WHITE(d + 1); fprintf(out, "escape:%s\n",
                                  n->u.var.escape ? "true" : "false");
            break;

        case A_kind_dec_type:
            fprintf(out, "dec_type(\n");
            WHITE(d + 1); fprintf(out, "name:%s\n",
                                  S_get_name(n->u.type.name));
            Ap_type(out, n->u.type.type, d + 1);
            break;

        case A_kind_dec_func:
            fprintf(out, "dec_function(\n");
            WHITE(d + 1); fprintf(out, "name:%s\n", S_get_name(n->u.func.name));
            Ap_para_list(out, n->u.func.paras, d + 1);
            if (n->u.func.ret) {
                WHITE(d + 1); fprintf(out, "return:%s\n",
                                      S_get_name(n->u.func.ret));
            }
            Ap_exp(out, n->u.func.body, d + 1);
            break;

        default:
            U_error(-1, "Unkown dec node");
    }
    WHITE(d); fprintf(out, ")\n");
}

static void Ap_exp(FILE *out, A_exp n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case A_kind_exp_var:
            fprintf(out, "exp_variable(\n");
            Ap_var(out, n->u.var, d + 1);
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
            Ap_exp_list(out, n->u.call.args, d + 1);
            break;

        case A_kind_exp_op:
            fprintf(out, "exp_op(\n");
            WHITE(d + 1); Ap_op(out, n->u.op.oper); fprintf(out, "\n");
            Ap_exp(out, n->u.op.left, d + 1);
            Ap_exp(out, n->u.op.right, d + 1);
            break;

        case A_kind_exp_array:
            fprintf(out, "exp_array(\n");
            WHITE(d + 1); fprintf(out, "array:%s\n",
                                  S_get_name(n->u.array.type));
            Ap_exp(out, n->u.array.size, d + 1);
            Ap_exp(out, n->u.array.init, d + 1);
            break;

        case A_kind_exp_record:
            fprintf(out, "exp_record(\n");
            Ap_arg_list(out, n->u.record.args, d + 1);
            break;

        case A_kind_exp_seq:
            fprintf(out, "exp_sequence(\n");
            Ap_exp_list(out, n->u.seq, d + 1);
            break;

        case A_kind_exp_assign:
            fprintf(out, "exp_assign(\n");
            Ap_var(out, n->u.assign.var, d + 1);
            Ap_exp(out, n->u.assign.exp, d + 1);
            break;

        case A_kind_exp_if:
            fprintf(out, "exp_if(\n");
            Ap_exp(out, n->u.if_.cond, d + 1);
            Ap_exp(out, n->u.if_.then, d + 1);
            if (n->u.if_.else_) {
                Ap_exp(out, n->u.if_.else_, d + 1);
            }
            break;

        case A_kind_exp_while:
            fprintf(out, "exp_while(\n");
            Ap_exp(out, n->u.while_.cond, d + 1);
            Ap_exp(out, n->u.while_.body, d + 1);
            break;

        case A_kind_exp_for:
            fprintf(out, "exp_for(\n");
            WHITE(d + 1); fprintf(out, "var:%s\n", S_get_name(n->u.for_.var));
            Ap_exp(out, n->u.for_.lo, d + 1);
            Ap_exp(out, n->u.for_.hi, d + 1);
            Ap_exp(out, n->u.for_.body, d + 1);
            WHITE(d + 1); fprintf(out, "escape:%s\n",
                                  n->u.for_.escape ? "true" : "false");
            break;

        case A_kind_exp_break:
            fprintf(out, "exp_break(\n");
            return;

        case A_kind_exp_let:
            fprintf(out, "exp_let(\n");
            Ap_dec_list(out, n->u.let.decs, d + 1);
            Ap_exp_list(out, n->u.let.body, d + 1);
            break;

        default:
            U_error(-1, "Unkown exp node");
    }
    WHITE(d); fprintf(out, ")\n");
}

static void Ap_var(FILE *out, A_var n, int d)
{
    if (n) {
        WHITE(d);
        switch(n->kind) {
            case A_kind_var_base:
                fprintf(out, "var_base(\n");
                WHITE(d + 1); fprintf(out, "base:%s\n",
                                      S_get_name(n->u.base.name));
                Ap_var(out, n->u.index.suffix, d + 1);
                return;

            case A_kind_var_index:
                fprintf(out, "var_array_index(\n");
                Ap_exp(out, n->u.index.exp, d + 1);
                Ap_var(out, n->u.index.suffix, d + 1);
                break;

            case A_kind_var_field:
                fprintf(out, "var_record_field(\n");
                WHITE(d + 1); fprintf(out, "field:%s\n",
                                      S_get_name(n->u.field.name));
                Ap_var(out, n->u.index.suffix, d + 1);
                break;

            default:
                U_error(-1, "Unkown var node");
        }
        WHITE(d); fprintf(out, ")\n");
    }
}

static void Ap_type(FILE *out, A_type n, int d)
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
            Ap_para_list(out, n->u.record, d + 1);
            break;

        default:
            U_error(-1, "Unkown type node");
    }
    WHITE(d); fprintf(out, ")\n");
}

static void Ap_dec_list(FILE *out, A_dec_list n, int d)
{
    WHITE(d); fprintf(out, "dec_list(");
    if (n) {
        fprintf(out, "\n");
        Ap_dec(out, n->head, d + 1);
        Ap_dec_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

static void Ap_exp_list(FILE *out, A_exp_list n, int d)
{
    WHITE(d); fprintf(out, "exp_list(");
    if (n) {
        fprintf(out, "\n");
        Ap_exp(out, n->head, d + 1);
        Ap_exp_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

static void Ap_para(FILE *out, A_para n, int d)
{
    WHITE(d); fprintf(out, "para(\n");
    WHITE(d + 1); fprintf(out, "var:%s\n", S_get_name(n->name));
    WHITE(d + 1); fprintf(out, "type:%s\n", S_get_name(n->type));
    WHITE(d + 1); fprintf(out, "escape:%s\n", n->escape ? "true" : "false");
    WHITE(d); fprintf(out, ")\n");
}

static void Ap_para_list(FILE *out, A_para_list n, int d)
{
    WHITE(d); fprintf(out, "para_list(");
    if (n) {
        fprintf(out, "\n");
        Ap_para(out, n->head, d + 1);
        Ap_para_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}

static void Ap_arg(FILE *out, A_arg n, int d)
{
    WHITE(d); fprintf(out, "arg(\n");
    WHITE(d + 1); fprintf(out, "field:%s\n", S_get_name(n->name));
    Ap_exp(out, n->exp, d + 1);
    WHITE(d); fprintf(out, ")\n");
}

static void Ap_arg_list(FILE *out, A_arg_list n, int d)
{
    WHITE(d); fprintf(out, "arg_list(");
    if (n) {
        fprintf(out, "\n");
        Ap_arg(out, n->head, d + 1);
        Ap_arg_list(out, n->tail, d + 1);
        WHITE(d);
    }
    fprintf(out, ")\n");
}


/********************************************************************************
 * Public Functions
 ********************************************************************************/

void A_pr_print(void *out, void *root)
{
    Ap_exp(out, root, 0);
}
