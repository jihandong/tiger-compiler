/****************************************************************************
 * Include Files
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

typedef enum
{
    Ak_op_plus,
    Ak_op_minus,
    Ak_op_times,
    Ak_op_divide,
    Ak_op_eq,
    Ak_op_neq,
    Ak_op_lt,
    Ak_op_le,
    Ak_op_gt,
    Ak_op_ge,
} Ak_op;

struct A_dec_ {
    Apos pos;

    enum
    {
        Ak_dec_var,
        Ak_dec_type,
        Ak_dec_func,
    } kind;

    union
    {
        struct { S_symbol var, type; A_exp init; bool escape; }                              var;
        struct { S_symbol type_s; A_type type; }                                             type;
        struct { Apos pos; S_symbol func; A_para_list params; S_symbol ret; A_exp body; }    func;
    } u;
};

struct A_exp_ {
    Apos pos;

    enum
    {
        Ak_exp_var,
        Ak_exp_nil,
        Ak_exp_int,
        Ak_exp_string,
        Ak_exp_call,
        Ak_exp_op,
        Ak_exp_record,
        Ak_exp_seq,
        Ak_exp_assign,
        Ak_exp_if,
        Ak_exp_while,
        Ak_exp_for,
        Ak_exp_break,
        Ak_exp_let,
        Ak_exp_array,
    } kind;

    union
    {
        A_var                                                     var;
        //                                                          nil;
        int                                                         int_;
        const char *                                                string_;
        struct { S_symbol func; A_exp_list args; }                  call;
        struct { Ak_op oper; A_exp left; A_exp right; }          op;
        struct { S_symbol record; A_argu_list members; }          record;
        A_exp_list                                                seq;
        struct { A_var var; A_exp exp; }                        assign;
        struct { A_exp cond, then, else_; }                       if_;
        struct { A_exp cond, body;}                               while_;
        struct { S_symbol var; A_exp lo, hi, body; bool escape; }   for_;
        //                                                          break;
        struct { A_dec_list decs; A_exp_list body; }            let;
        struct { S_symbol array; A_exp size, init; }                array;
    } u;
};

struct A_var_ {
    Apos pos;

    enum
    {
        Ak_var,
        Ak_var_slice,  // array slice
        Ak_var_member, // record member
    } kind;

    union
    {
        struct { S_symbol base; A_var suffix; }     base;
        struct { A_exp exp; A_var suffix; }     slice;
        struct { S_symbol member; A_var suffix; }   member;
    } u;
};

struct A_type_ {
    Apos pos;

    enum
    {
        Ak_type_var,
        Ak_type_array,
        Ak_type_record,
    } kind;

    union
    {
        S_symbol          var;
        S_symbol          array;
        A_para_list record;
    } u;
};

struct A_dec_list_            { A_dec       head; A_dec_list      tail; };
struct A_exp_list_            { A_exp       head; A_exp_list      tail; };
struct A_para_list_     { A_para    head; A_para_list   tail; };
struct A_argu_list_   { A_argu    head; A_argu_list   tail; };
struct A_para_          { S_symbol var, type; Apos pos; bool escape; };
struct A_argu_        { S_symbol var; A_exp exp; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

A_dec Am_dec_var(Apos pos, S_symbol var, S_symbol type, A_exp init);
A_dec Am_dec_type(S_symbol type_s, A_type type);
A_dec Am_dec_func(Apos pos, S_symbol func, A_para_list params, S_symbol ret, A_exp body);

A_exp Am_exp_var(Apos pos, A_var var);
A_exp Am_exp_nil(Apos pos);
A_exp Am_exp_int(Apos pos, int i);
A_exp Am_exp_string(Apos pos, const char *s);
A_exp Am_exp_call(Apos pos, S_symbol func, A_exp_list args);
A_exp Am_exp_op(Apos pos, Ak_op oper, A_exp left, A_exp right);
A_exp Am_exp_record(Apos pos, S_symbol record, A_argu_list members);
A_exp Am_exp_seq(Apos pos, A_exp_list seq);
A_exp Am_exp_assign(Apos pos, A_var var, A_exp exp);
A_exp Am_exp_if(Apos pos, A_exp test, A_exp then, A_exp else_);
A_exp Am_exp_while(Apos pos, A_exp test, A_exp body);
A_exp Am_exp_for(Apos pos, S_symbol var, A_exp lo, A_exp hi, A_exp body);
A_exp Am_exp_break(Apos pos);
A_exp Am_exp_let(Apos pos, A_dec_list decs, A_exp_list body);
A_exp Am_exp_array(Apos pos, S_symbol array, A_exp size, A_exp init);

A_var Am_var(Apos pos, S_symbol base, A_var suffix);
A_var Am_var_slice(Apos pos, A_exp exp, A_var suffix);
A_var Am_var_member(Apos pos, S_symbol member, A_var suffix);

A_type Am_type_var(Apos pos, S_symbol name);
A_type Am_type_array(Apos pos, S_symbol array);
A_type Am_type_record(Apos pos, A_para_list record);

A_dec_list Am_dec_list(A_dec head, A_dec_list tail);
A_exp_list Am_exp_list(A_exp head, A_exp_list tail);
A_para_list Am_para_list(A_para head, A_para_list tail);
A_argu_list Am_argu_list(A_argu head, A_argu_list tail);
A_para Am_para(Apos pos, S_symbol var, S_symbol type);
A_argu Am_argu(S_symbol var, A_exp exp);

/**
 * print abstract syntax tree.
 * @param[in] out   output file(FILE *).
 * @param[in] root  root node(A_exp).
 */
void Af_print(void *out, void *root);
