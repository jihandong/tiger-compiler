/********************************************************************************
 * Include Files
 ********************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol.h"
#include "util.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

// print white spaces
#define WHITE(x) ({ int i; for(i = 0; i <= x; i++) fprintf(out, " "); })

static const char * kind_op[] =
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

static void ast_pr_dec(FILE *out, ast_dec n, int d);
static void ast_pr_exp(FILE *out, ast_exp n, int d);
static void ast_pr_var(FILE *out, ast_var n, int d);
static void ast_pr_type(FILE *out, ast_type n, int d);
static void ast_pr_dec_list(FILE *out, ast_dec_list n, int d);
static void ast_pr_exp_list(FILE *out, ast_exp_list n, int d);
static void ast_pr_tfield(FILE *out, ast_tfield n, int d);
static void ast_pr_tfield_list(FILE *out, ast_tfield_list n, int d);
static void ast_pr_rfield(FILE *out, ast_rfield n, int d);
static void ast_pr_rfield_list(FILE *out, ast_rfield_list n, int d);

/********************************************************************************
 * Private Functions
 ********************************************************************************/

static inline void indent(FILE *out, int d) {
    int i;
    for(i = 0; i <= d; i++) fprintf(out, " ");
}

static inline void ast_pr_op(FILE *out, ast_op op)
{
    fprintf(out, "%s", kind_op[op]);
}

static void ast_pr_dec(FILE *out, ast_dec n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case kind_dec_var:
            fprintf(out, "dec_variable(\n");
            if (n->u.var.type) {
                WHITE(d + 1); fprintf(out, "type:%s", sym_get_name(n->u.var.type));
            }
            ast_pr_exp(out, n->u.var.init, d + 1);
            WHITE(d + 1); fprintf(out, "escape%s\n", n->u.var.escape ? "true" : "false");
            break;

        case kind_dec_type:
            fprintf(out, "dec_type(\n");
            WHITE(d + 1); fprintf(out, "name:%s\n", sym_get_name(n->u.type.type_s));
            ast_pr_type(out, n->u.type.type, d + 1);
            break;

        case kind_dec_func:
            fprintf(out, "dec_function(\n");
            WHITE(d + 1); fprintf(out, "name:%s\n", sym_get_name(n->u.func.func));
            ast_pr_tfield_list(out, n->u.func.params, d + 1);
            if (n->u.func.ret) {
                WHITE(d + 1); fprintf(out, "return:%s\n", sym_get_name(n->u.func.ret));
            }
            ast_pr_exp(out, n->u.func.body, d + 1);
            break;

        default:
            assert(0);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_exp(FILE *out, ast_exp n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case kind_exp_var:
            fprintf(out, "exp_variable(\n");
            ast_pr_var(out, n->u.var, d + 1);
            break;

        case kind_exp_nil:
            fprintf(out, "exp_nil()\n");
            return;

        case kind_exp_int:
            fprintf(out, "exp_integer(%d)\n", n->u.int_);
            return;

        case kind_exp_string:
            fprintf(out, "exp_string(%s)\n", n->u.string_);
            return;

        case kind_exp_call:
            fprintf(out, "exp_call(\n");
            WHITE(d + 1); fprintf(out, "func:%s\n", sym_get_name(n->u.call.func));
            ast_pr_exp_list(out, n->u.call.args, d + 1);
            break;

        case kind_exp_op:
            fprintf(out, "exp_op(\n");
            WHITE(d + 1); ast_pr_op(out, n->u.op.op); fprintf(out, "\n");
            ast_pr_exp(out, n->u.op.left, d + 1);
            ast_pr_exp(out, n->u.op.right, d + 1);
            break;

        case kind_exp_record:
            fprintf(out, "exp_record(\n");
            ast_pr_rfield_list(out, n->u.record.members, d + 1); fprintf(out, "\n");
            break;

        case kind_exp_seq:
            fprintf(out, "exp_sequence(\n");
            ast_pr_exp_list(out, n->u.seq, d + 1);
            break;

        case kind_exp_assign:
            fprintf(out, "exp_assign(\n");
            ast_pr_var(out, n->u.assign.var, d + 1);
            ast_pr_exp(out, n->u.assign.exp, d + 1);
            break;

        case kind_exp_if:
            fprintf(out, "exp_if(\n");
            ast_pr_var(out, n->u.if_.cond, d + 1);
            ast_pr_exp(out, n->u.if_.then, d + 1);
            if (n->u.if_.else_) {
                ast_pr_exp(out, n->u.if_.else_, d + 1);
            }
            break;

        case kind_exp_while:
            fprintf(out, "exp_while(\n");
            ast_pr_exp(out, n->u.while_.cond, d + 1);
            ast_pr_exp(out, n->u.while_.body, d + 1);
            break;

        case kind_exp_for:
            fprintf(out, "exp_for(\n");
            ast_pr_var(out, n->u.for_.var, d + 1);
            ast_pr_exp(out, n->u.for_.lo, d + 1);
            ast_pr_exp(out, n->u.for_.hi, d + 1);
            ast_pr_exp(out, n->u.for_.body, d + 1);
            WHITE(d + 1); fprintf(out, "escape:%s\n", n->u.for_.escape ? "true" : "false");
            break;

        case kind_exp_break:
            fprintf(out, "exp_break(\n");
            return;

        case kind_exp_let:
            fprintf(out, "exp_let(\n");
            ast_pr_dec_list(out, n->u.let.decs, d + 1);
            ast_pr_exp_list(out, n->u.let.body, d + 1);
            break;

        case kind_exp_array:
            fprintf(out, "exp_array(\n");
            WHITE(d + 1); fprintf(out, "%s\n", sym_get_name(n->u.array.array));
            ast_pr_exp(out, n->u.array.size, d + 1);
            ast_pr_exp(out, n->u.array.init, d + 1);
            break;

        default:
            assert(0);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_var(FILE *out, ast_var n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case kind_var:
            fprintf(out, "var_simple(\n%s", sym_get_name(n->u.var));
            break;

        case kind_var_slice:
            fprintf(out, "var_array_slice(\n");
            ast_pr_var(out, n->u.slice.var, d + 1);
            ast_pr_exp(out, n->u.slice.exp, d + 1);
            break;

        case kind_var_member:
            fprintf(out, "var_record_member(\n", sym_get_name(n->u.var));
            ast_pr_var(out, n->u.member.var, d + 1);
            WHITE(d + 1); fprintf("member:%s\n", sym_get_name(n->u.member.member));
            break;

        default:
            assert(0);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_type(FILE *out, ast_type n, int d)
{
    WHITE(d);
    switch(n->kind) {
        case kind_type_var:
            fprintf(out, "type_variable(\n%s\n", sym_get_name(n->u.var));
            break;

        case kind_type_array:
            fprintf(out, "type_array(\n%s\n", sym_get_name(n->u.array));
            break;

        case kind_type_record:
            fprintf(out, "type_record(\n");
            ast_pr_rfield_list(out, n->u.record, d + 1);
            break;

        default:
            assert(0);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_dec_list(FILE *out, ast_dec_list n, int d)
{
    WHITE(d); fprintf(out, "dec_list(\n");
    if (n) {
        ast_pr_dec(out, n->head, d + 1);
        ast_pr_dec_list(out, n->tail, d + 1);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_exp_list(FILE *out, ast_exp_list n, int d)
{
    WHITE(d); fprintf(out, "exp_list(\n");
    if (n) {
        ast_pr_exp(out, n->head, d + 1);
        ast_pr_exp_list(out, n->tail, d + 1);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_tfield(FILE *out, ast_tfield n, int d)
{

}

static void ast_pr_tfield_list(FILE *out, ast_tfield_list n, int d)
{
    WHITE(d); fprintf(out, "type_field_list(\n");
    if (n) {
        ast_pr_tfield(out, n->head, d + 1);
        ast_pr_tfield_list(out, n->tail, d + 1);
    }
    WHITE(d); fprintf(out, ")\n");
}

static void ast_pr_rfield(FILE *out, ast_rfield n, int d)
{

}

static void ast_pr_rfield_list(FILE *out, ast_rfield_list n, int d)
{
    WHITE(d); fprintf(out, "record_field_list(\n");
    if (n) {
        ast_pr_rfield(out, n->head, d + 1);
        ast_pr_rfield_list(out, n->tail, d + 1);
    }
    WHITE(d); fprintf(out, ")\n");
}


/********************************************************************************
 * Public Functions
 ********************************************************************************/

void ast_pr(void *out, void *root)
{
    ast_pr_exp(out, root, 0);
}
