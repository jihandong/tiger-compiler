#include "display.h"

static void pr_ast_var(FILE *target, ast_var node, int blank);
static void pr_ast_dec(FILE *target, ast_dec node, int blank);
static void pr_ast_type(FILE *target, ast_type node, int blank);
static void pr_ast_field(FILE *target, ast_field node, int blank);
static void pr_ast_field_list(FILE *target, ast_field_list node, int blank);
static void pr_ast_exp_list(FILE *target, ast_exp_list node, int blank);
static void pr_ast_func_dec(FILE *target, ast_func_dec node, int blank);
static void pr_ast_func_dec_list(FILE *target, ast_func_dec_list node, int blank);
static void pr_ast_dec_list(FILE *target, ast_dec_list node, int blank);
static void pr_ast_type_dec(FILE *target, ast_type_dec node, int blank);
static void pr_ast_type_dec_list(FILE *target, ast_type_dec_list node, int blank);
static void pr_ast_efield(FILE *target, ast_efield node, int blank);
static void pr_ast_efield_list(FILE *target, ast_efield_list node, int blank);

static inline void indent(FILE *target, int blank)
{
    int i;
    for (i = 0; i < blank; i++) fprintf(target, " ");
}

static void pr_ast_var(FILE *target, ast_var node, int blank)
{
    indent(target, blank);
    switch (node->kind) {
        case kind_ast_var_trival:
            fprintf(target, "trival variable(%s)", node->u.trival.name);
            break;

        case kind_ast_var_field:
            fprintf(target, "field variable(\n");
            pr_var(target, node->u.field.var, blank + 1); fprintf(target, ",\n");
            indent(target, blank + 1); fprintf(target, "%s)", node->u.field.sym.name);
            break;

        case kind_ast_var_subscript:
            fprintf(target, "subscript variable(\n");
            pr_vr(target, node->u.subscript.var, blank + 1); fprintf(target, ",\n");
            pr_exp(target, node->u.subscript.exp, blank + 1); fprintf(target, ")");
            break;

        default:
            assert(0);
    }
}

void pr_ast_exp(FILE *target, ast_exp node, int blank)
{
    indent(target, blank);
    switch (node->kind) {
        case kind_ast_exp_var:
            fprintf(target, "variable expression(\n");
            pr_var(target, node->u.var, blank + 1);
            fprintf(target, ")");
            break;

        case kind_ast_exp_nil:
            fprintf(target, "nil expression()");
            break;

        case kind_ast_exp_int:
            fprintf(target, "integer expression(%d)", node->u.int_);
            break;

        case kind_ast_exp_string:
            fprintf(target, "string expression(%s)", node->u.string_);
            break;

        case kind_ast_exp_call:
            fprintf(target, "call expression(%s,", node->u.call.func.name);
            pr_ast_exp_list(target, node->u.call.args, blank + 1); fprintf(target, ")");
            break;

        case kind_ast_exp_op:
            fprintf(target, "op expression(\n");
            indent(target, blank + 1); pr_ast_op(target, node->u.op.op); fprintf(target, ",\n"); 
            pr_ast_exp(target, node->u.op.left,  blank + 1); fprintf(target, ",\n");
            pr_ast_exp(target, node->u.op.right, blank + 1); fprintf(target, ")");
            break;

        case kind_ast_exp_record:
        case kind_ast_exp_seq:
        case kind_ast_exp_assign:
        case kind_ast_exp_if:
        case kind_ast_exp_while:
        case kind_ast_exp_for:
        case kind_ast_exp_break:
        case kind_ast_exp_let:
        case kind_ast_exp_array:
        default:
            assert(0);
    }
}