/********************************************************************************
 * Includes
 ********************************************************************************/

#include "ast.h"
#include "symbol.h"
#include "type.h"

/********************************************************************************
 * Definitions
 ********************************************************************************/

typedef void *ir_exp; /*< ir not implemented yet */

static struct sem_exp { ir_exp exp; ty_type type; };

/********************************************************************************
 * Privates
 ********************************************************************************/

static sem_exp sem_mk_exp(ir_exp exp, ty_type type)
{
    struct sem_exp e;

    e.exp   = exp;
    e.type  = type;

    return e;
}

/********************************************************************************
 * Publics
 ********************************************************************************/

void sem_trans_dec(symbol_table venv, symbol_table tenv, ast_var n)
{
    switch(n->kind) {
        case kind_var: {
            
        }

        case kind_var_slice: {
            
        }

        case kind_var_memberr: {
            
        }

        default:
    }
}

struct sem_exp sem_trans_exp(symbol_table venv, symbol_table tenv, ast_exp n)
{
    switch(n->kind) {
        case kind_ast_op: {
            struct sem_exp left  = sem_trans_exp(venv, tenv, n->u.op.left);
            struct sem_exp right = sem_trans_exp(venv, tenv, n->u.op.right);

            if (left.type->kind != kind_ty_int)
                util_err(n->u.op.left->pos, "require int in exp op");
            if (right.type->kind != kind_ty_int)
                util_err(n->u.op.right->pos, "require int in exp op");

            return sem_mk_exp(NULL, ty_mk_int());
        }

        case kind_exp_let: {
            struct sem_exp exp;
            ast_dec_list d;

            sym_table_begin(venv);
            sym_table_begin(tenv);

            for(d = n->u.let.decs; d; d = d->next)
                sem_trans_dec(venv, tenv, d->head);
            exp = sem_trans_exp(venv, tenv, n->u.let.body);

            sym_table_end(tenv);
            sym_table_end(venv);

            return exp;
        }

        default:
    }
}

struct sem_exp sem_trans_var(symbol_table venv, symbol_table tenv, ast_var n)
{
    switch(n->kind) {
        case:
        default:
    }
}

ty_type sem_exp sem_trans_type(symbol_table tenv, ast_type n)
{
    switch(n->kind) {
        case:
        default:
    }
}









