#pragma once

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct AST_dec_ *       AST_dec;
typedef struct AST_exp_ *       AST_exp;
typedef struct AST_var_ *       AST_var;
typedef struct AST_type_ *      AST_type;
typedef struct AST_para_ *      AST_para; /* func def and record def */
typedef struct AST_para_list_ * AST_para_list;
typedef struct AST_arg_ *       AST_arg; /* func call and record create */
typedef struct AST_arg_list_ *  AST_arg_list;
typedef struct AST_dec_list_ *  AST_dec_list;
typedef struct AST_exp_list_ *  AST_exp_list;

typedef int Apos;

typedef enum {
    AST_kind_op_plus,
    AST_kind_op_minus,
    AST_kind_op_times,
    AST_kind_op_divide,
    AST_kind_op_eq,
    AST_kind_op_neq,
    AST_kind_op_lt,
    AST_kind_op_le,
    AST_kind_op_gt,
    AST_kind_op_ge,
} AST_kind_op;

struct AST_dec_
{
    Apos pos;

    enum {
        AST_kind_dec_var,
        AST_kind_dec_type,
        AST_kind_dec_func,
    } kind;

    union {
        struct { SYM_symbol name, type; AST_exp init; bool escape; }  var;
        struct { SYM_symbol name; AST_type type; }                    type;
        struct {
            Apos pos;
            SYM_symbol name, ret;
            AST_para_list paras;
            AST_exp body;
        } func;
    } u;
};

struct AST_exp_
{
    Apos pos;

    enum {
        AST_kind_exp_var,
        AST_kind_exp_nil,
        AST_kind_exp_int,
        AST_kind_exp_str,
        AST_kind_exp_call,
        AST_kind_exp_op,
        AST_kind_exp_array,
        AST_kind_exp_record,
        AST_kind_exp_seq,
        AST_kind_exp_assign,
        AST_kind_exp_if,
        AST_kind_exp_while,
        AST_kind_exp_for,
        AST_kind_exp_break,
        AST_kind_exp_let,
    } kind;

    union {
        AST_var                                                     var;
        //                                                          nil;
        int                                                         int_;
        const char *                                                str_;
        struct { SYM_symbol func; AST_exp_list args; }              call;
        struct { AST_kind_op oper; AST_exp left; AST_exp right; }   op;
        struct { SYM_symbol type; AST_exp size, init; }             array;
        struct { SYM_symbol type; AST_arg_list args; }              record;
        AST_exp_list                                                seq;
        struct { AST_var var; AST_exp exp; }                        assign;
        struct { AST_exp cond, then, else_; }                       if_;
        struct { AST_exp cond, body;}                               while_;
        struct { SYM_symbol var; AST_exp lo, hi, body; bool escape; } for_;
        //                                                          break;
        struct { AST_dec_list decs; AST_exp_list body; }            let;
    } u;
};

struct AST_var_
{
    Apos pos;

    enum {
        AST_kind_var_base,
        AST_kind_var_index,
        AST_kind_var_field,
    } kind;

    union {
        struct { SYM_symbol name; AST_var suffix; } base;
        struct { AST_exp exp; AST_var suffix; }     index; /*< array index */
        struct { SYM_symbol name; AST_var suffix; } field; /*< record fields */
    } u;
};

struct AST_type_
{
    Apos pos;

    enum {
        AST_kind_type_name,
        AST_kind_type_array,
        AST_kind_type_record,
    } kind;

    union {
        SYM_symbol    name;
        SYM_symbol    array;
        AST_para_list record;
    } u;
};

struct AST_arg_         { SYM_symbol name; AST_exp exp; };
struct AST_arg_list_    { AST_arg    head; AST_arg_list    tail; };
struct AST_para_        { SYM_symbol name, type; Apos pos; bool escape; };
struct AST_para_list_   { AST_para   head; AST_para_list   tail; };
struct AST_dec_list_    { AST_dec    head; AST_dec_list    tail; };
struct AST_exp_list_    { AST_exp    head; AST_exp_list    tail; };

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
AST_dec AST_mk_dec_var(Apos pos, SYM_symbol name, SYM_symbol type, AST_exp init);
/**
 * make type declare astnode.
 * @param[in] pos
 * @param[in] name  type name symbol.
 * @param[in] type  type definition astnode.
 * @return new astnode.
 */
AST_dec AST_mk_dec_type(SYM_symbol name, AST_type type);
/**
 * make function declare astnode.
 * @param[in] pos
 * @param[in] name  function symbol.
 * @param[in] paras parameter list, each parameter has name and type.
 * @param[in] ret   return type symbol.
 * @param[in] body  functions body expressions.
 * @return new astnode.
 */
AST_dec AST_mk_dec_func(Apos pos, SYM_symbol name, AST_para_list paras,
                        SYM_symbol ret, AST_exp body);

/****************************************************************************
 * Public: expression constructor
 ****************************************************************************/

/**
 * make left value expression astnode.
 * @param[in] pos
 * @param[in] var   left value.
 * @return new astnode.
 */
AST_exp AST_mk_exp_var(Apos pos, AST_var var);
/**
 * make nil astnode.
 * @param[in] pos
 * @return new astnode.
 */
AST_exp AST_mk_exp_nil(Apos pos);
/**
 * make const integer astnode.
 * @param[in] pos
 * @param[in] i     const integer.
 * @return new astnode.
 */
AST_exp AST_mk_exp_int(Apos pos, int i);
/**
 * make const string astnode.
 * @param[in] pos
 * @param[in] s     const string.
 * @return new astnode.
 */
AST_exp AST_mk_exp_str(Apos pos, const char *s);
/**
 * make function calling astnode.
 * @param[in] pos
 * @param[in] func  function symbol.
 * @param[in] args  argment expression list.
 * @return new astnode.
 */
AST_exp AST_mk_exp_call(Apos pos, SYM_symbol func, AST_exp_list args);
/**
 * make "op1 op op2" astnode.
 * @param[in] pos
 * @param[in] oper  operator type: + - * /.
 * @param[in] left  left operand expression.
 * @param[in] right right operand expression.
 * @return new astnode.
 */
AST_exp AST_mk_exp_op(Apos pos, AST_kind_op oper,
                      AST_exp left, AST_exp right);
/**
 * make array creating astnode.
 * @param[in] pos
 * @param[in] type  array type (not element).
 * @param[in] size  array size.
 * @param[in] init  array element initial value.
 * @return new astnode.
 */
AST_exp AST_mk_exp_array(Apos pos, SYM_symbol type, AST_exp size,
                         AST_exp init);
/**
 * make record creating astnode.
 * @param[in] pos
 * @param[in] type  record type.
 * @param[in] args  field initialize.
 * @return new astnode.
 */
AST_exp AST_mk_exp_record(Apos pos, SYM_symbol type, AST_arg_list args);
/**
 * make expression sequence astnode.
 * @param[in] pos
 * @param[in] seq   expression sequence.
 * @return new astnode.
 */
AST_exp AST_mk_exp_seq(Apos pos, AST_exp_list seq);
/**
 * make variable assignment astnode.
 * @param[in] pos
 * @param[in] var   left value.
 * @param[in] exp   expression.
 * @return new astnode.
 */
AST_exp AST_mk_exp_assign(Apos pos, AST_var var, AST_exp exp);
/**
 * make if-then, if-then-else, and, or astnode.
 * @param[in] pos
 * @param[in] cond  condition.
 * @param[in] then  expression for true condition.
 * @param[in] else_ expression for false confition, can be null.
 * @return new astnode.
 */
AST_exp AST_mk_exp_if(Apos pos, AST_exp cond, AST_exp then, AST_exp else_);
/**
 * make while loop astnode.
 * @param[in] pos
 * @param[in] cond  condition.
 * @param[in] body  loop body.
 * @return new astnode.
 */
AST_exp AST_mk_exp_while(Apos pos, AST_exp cond, AST_exp body);
/**
 * make for loop astnode.
 * @param[in] pos
 * @param[in] var   temp variable.
 * @param[in] lo    lowest value.
 * @param[in] hi    highest value.
 * @param[in] body  loop body.
 * @return new astnode.
 */
AST_exp AST_mk_exp_for(Apos pos, SYM_symbol var, AST_exp lo, AST_exp hi,
                       AST_exp body);
/**
 * make break astnode.
 * @param[in] pos
 * @return new astnode.
 */
AST_exp AST_mk_exp_break(Apos pos);
/**
 * make let ... in ... astnode
 * @param[in] pos
 * @param[in] decs  declarations.
 * @param[in] body  expression sequence.
 * @return new astnode.
 */
AST_exp AST_mk_exp_let(Apos pos, AST_dec_list decs, AST_exp_list body);

/****************************************************************************
 * Public: variables constructor
 ****************************************************************************/

/**
 * make base left value astnode.
 * @param[in] pos
 * @param[in] name      left value bast address.
 * @param[in] suffix    array index or record field.
 * @return new astnode.
 */
AST_var AST_mk_var_base(Apos pos, SYM_symbol name, AST_var suffix);
/**
 * make array index suffix astnode.
 * @param[in] pos
 * @param[in] exp       array index.
 * @param[in] suffix    more array index or record field.
 * @return new astnode.
 */
AST_var AST_mk_var_index(Apos pos, AST_exp exp, AST_var suffix);
/**
 * make field member suffix astnode.
 * @param[in] pos
 * @param[in] field     record field.
 * @param[in] suffix    array index or record field.
 * @return new astnode.
 */
AST_var AST_mk_var_field(Apos pos, SYM_symbol field, AST_var suffix);

/****************************************************************************
 * Public: type definition constructor
 ****************************************************************************/

/**
 * make type name definition astnode.
 * @param[in] pos
 * @param[in] name      type name.
 * @return new astnode.
 */
AST_type AST_mk_type_name(Apos pos, SYM_symbol name);
/**
 * make array type name definition astnode.
 * @param[in] pos
 * @param[in] array     array element type name.
 * @return new astnode.
 */
AST_type AST_mk_type_array(Apos pos, SYM_symbol array);
/**
 * make record definition astnode.
 * @param[in] pos
 * @param[in] fields    record fields' definition.
 * @return new astnode.
 */
AST_type AST_mk_type_record(Apos pos, AST_para_list fields);

/****************************************************************************
 * Public: link list constructor
 ****************************************************************************/

/**
 * make declaration link list node.
 * @param[in] head
 * @param[in] tail
 * @return new astnode.
 */
AST_dec_list AST_mk_dec_list(AST_dec head, AST_dec_list tail);
/**
 * make expression link list node.
 * @param[in] head
 * @param[in] tail
 * @return new astnode.
 */
AST_exp_list AST_mk_exp_list(AST_exp head, AST_exp_list tail);
/**
 * make parameter link list node.
 * @param[in] head
 * @param[in] tail
 * @return new astnode.
 */
AST_para_list AST_mk_para_list(AST_para head, AST_para_list tail);
/**
 * make argument link list node.
 * @param[in] head
 * @param[in] tail
 * @return new astnode.
 */
AST_arg_list AST_mk_arg_list(AST_arg head, AST_arg_list tail);
/**
 * make parameter astnode.
 * @param[in] pos
 * @param[in] var 
 * @param[in] type 
 * @return new astnode.
 */
AST_para AST_mk_para(Apos pos, SYM_symbol var, SYM_symbol type);
/**
 * make argument astnode.
 * @param[in] var 
 * @param[in] exp 
 * @return new astnode.
 */
AST_arg AST_mk_arg(SYM_symbol var, AST_exp exp);

/****************************************************************************
 * Public: tool function
 ****************************************************************************/

/**
 * print abstract syntax tree.
 * @param[in] out   output file.
 * @param[in] root  root node.
 */
void AST_print(FILE *out, AST_exp root);
