/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>
#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct A_dec_ *         A_dec;
typedef struct A_exp_ *         A_exp;
typedef struct A_var_ *         A_var;
typedef struct A_type_ *        A_type;
typedef struct A_dec_list_ *    A_dec_list;
typedef struct A_exp_list_ *    A_exp_list;
typedef struct A_para_ *        A_para;
typedef struct A_para_list_ *   A_para_list;
typedef struct A_argu_ *        A_argu;
typedef struct A_argu_list_ *   A_argu_list;

typedef int Apos;

typedef enum {
    A_kind_op_plus,
    A_kind_op_minus,
    A_kind_op_times,
    A_kind_op_divide,
    A_kind_op_eq,
    A_kind_op_neq,
    A_kind_op_lt,
    A_kind_op_le,
    A_kind_op_gt,
    A_kind_op_ge,
} A_kind_op;

struct A_dec_
{
    Apos pos;

    enum {
        A_kind_dec_var,
        A_kind_dec_type,
        A_kind_dec_func,
    } kind;

    union {
        struct { S_symbol name, type; A_exp init; bool escape; }    var;
        struct { S_symbol name; A_type type; }                      type;
        struct {
            Apos pos; S_symbol name; A_para_list paras; S_symbol ret; A_exp body;
        } func;
    } u;
};

struct A_exp_
{
    Apos pos;

    enum {
        A_kind_exp_var,
        A_kind_exp_nil,
        A_kind_exp_int,
        A_kind_exp_str,
        A_kind_exp_call,
        A_kind_exp_op,
        A_kind_exp_array,
        A_kind_exp_record,
        A_kind_exp_seq,
        A_kind_exp_assign,
        A_kind_exp_if,
        A_kind_exp_while,
        A_kind_exp_for,
        A_kind_exp_break,
        A_kind_exp_let,
    } kind;

    union {
        A_var                                                       var;
        //                                                          nil;
        int                                                         int_;
        const char *                                                str_;
        struct { S_symbol func; A_exp_list args; }                  call;
        struct { A_kind_op oper; A_exp left; A_exp right; }         op;
        struct { S_symbol type; A_exp size, init; }                 array;
        struct { S_symbol type; A_argu_list fields; }               record;
        A_exp_list                                                  seq;
        struct { A_var var; A_exp exp; }                            assign;
        struct { A_exp cond, then, else_; }                         if_;
        struct { A_exp cond, body;}                                 while_;
        struct { S_symbol var; A_exp lo, hi, body; bool escape; }   for_;
        //                                                          break;
        struct { A_dec_list decs; A_exp_list body; }                let;
    } u;
};

struct A_var_
{
    Apos pos;

    enum {
        A_kind_var_base,
        A_kind_var_slice, // array slice
        A_kind_var_field, // record field
    } kind;

    union {
        struct { S_symbol base; A_var suffix; }     base;
        struct { A_exp exp; A_var suffix; }         slice;
        struct { S_symbol field; A_var suffix; }    field;
    } u;
};

struct A_type_
{
    Apos pos;

    enum {
        A_kind_type_name,
        A_kind_type_array,
        A_kind_type_record,
    } kind;

    union {
        S_symbol    name;
        S_symbol    array;
        A_para_list record;
    } u;
};

struct A_dec_list_      { A_dec     head; A_dec_list    tail; };
struct A_exp_list_      { A_exp     head; A_exp_list    tail; };
struct A_para_list_     { A_para    head; A_para_list   tail; };
struct A_argu_list_     { A_argu    head; A_argu_list   tail; };
struct A_para_          { S_symbol name, type; Apos pos; bool escape; };
struct A_argu_          { S_symbol name; A_exp exp; };

/****************************************************************************
 * Public: declaration constructor
 ****************************************************************************/

/**
 * make variable declare astnode.
 * @param[in] pos
 * @param[in] name  variable symbol.
 * @param[in] type  type symbol.
 * @param[in] init  init expression.
 * @return new astnode.
 */
A_dec A_mk_dec_var(Apos pos, S_symbol name, S_symbol type, A_exp init);
/**
 * make type declare astnode.
 * @param[in] pos
 * @param[in] name  type name symbol.
 * @param[in] type  type definition astnode.
 * @return new astnode.
 */
A_dec A_mk_dec_type(S_symbol name, A_type type);
/**
 * make function declare astnode.
 * @param[in] pos
 * @param[in] name  function symbol.
 * @param[in] paras parameter list, each parameter has name and type.
 * @param[in] ret   return type symbol.
 * @param[in] body  functions body expressions.
 * @return new astnode.
 */
A_dec A_mk_dec_func(Apos pos, S_symbol name, A_para_list paras, S_symbol ret,
                    A_exp body);

/****************************************************************************
 * Public: expression constructor
 ****************************************************************************/

/**
 * make left value expression astnode.
 * @param[in] pos
 * @param[in] var   left value.
 * @return new astnode.
 */
A_exp A_mk_exp_var(Apos pos, A_var var);
/**
 * make nil astnode.
 * @param[in] pos
 * @return new astnode.
 */
A_exp A_mk_exp_nil(Apos pos);
/**
 * make const integer astnode.
 * @param[in] pos
 * @param[in] i     const integer.
 * @return new astnode.
 */
A_exp A_mk_exp_int(Apos pos, int i);
/**
 * make const string astnode.
 * @param[in] pos
 * @param[in] s     const string.
 * @return new astnode.
 */
A_exp A_mk_exp_str(Apos pos, const char *s);
/**
 * make function calling astnode.
 * @param[in] pos
 * @param[in] func  function symbol.
 * @param[in] args  argument expression list.
 * @return new astnode.
 */
A_exp A_mk_exp_call(Apos pos, S_symbol func, A_exp_list args);
A_exp A_mk_exp_op(Apos pos, A_kind_op oper, A_exp left, A_exp right);
A_exp A_mk_exp_array(Apos pos, S_symbol name, A_exp size, A_exp init);
A_exp A_mk_exp_record(Apos pos, S_symbol name, A_argu_list fields);
A_exp A_mk_exp_seq(Apos pos, A_exp_list seq);
A_exp A_mk_exp_assign(Apos pos, A_var var, A_exp exp);
A_exp A_mk_exp_if(Apos pos, A_exp cond, A_exp then, A_exp else_);
A_exp A_mk_exp_while(Apos pos, A_exp cond, A_exp body);
A_exp A_mk_exp_for(Apos pos, S_symbol var, A_exp lo, A_exp hi, A_exp body);
A_exp A_mk_exp_break(Apos pos);
A_exp A_mk_exp_let(Apos pos, A_dec_list decs, A_exp_list body);

/****************************************************************************
 * Public: variables constructor
 ****************************************************************************/

A_var A_mk_var_base(Apos pos, S_symbol base, A_var suffix);
A_var A_mk_var_slice(Apos pos, A_exp exp, A_var suffix);
A_var A_mk_var_field(Apos pos, S_symbol field, A_var suffix);

/****************************************************************************
 * Public: type definition constructor
 ****************************************************************************/

A_type A_mk_type_var(Apos pos, S_symbol name);
A_type A_mk_type_array(Apos pos, S_symbol name);
A_type A_mk_type_record(Apos pos, A_para_list fields);

/****************************************************************************
 * Public: link list constructor
 ****************************************************************************/

A_dec_list A_mk_dec_list(A_dec head, A_dec_list tail);
A_exp_list A_mk_exp_list(A_exp head, A_exp_list tail);
A_para_list A_mk_para_list(A_para head, A_para_list tail);
A_argu_list A_mk_argu_list(A_argu head, A_argu_list tail);
A_para A_mk_para(Apos pos, S_symbol var, S_symbol type);
A_argu A_mk_argu(S_symbol var, A_exp exp);

/****************************************************************************
 * Public: display function
 ****************************************************************************/

/**
 * print abstract syntax tree.
 * @param[in] out   output file(FILE *).
 * @param[in] root  root node(A_exp).
 */
void A_pr_print(void *out, void *root);
