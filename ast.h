/****************************************************************************
 * Include Files
 ****************************************************************************/

#include <stdbool.h>
#include "symbol.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

typedef struct ast_dec_ *                   ast_dec;
typedef struct ast_exp_ *                   ast_exp;
typedef struct ast_var_ *                   ast_var;
typedef struct ast_type_ *                  ast_type;
typedef struct ast_dec_list_ *              ast_dec_list;
typedef struct ast_exp_list_ *              ast_exp_list;
typedef struct ast_type_field_ *            ast_tfield;
typedef struct ast_type_field_list_ *       ast_tfield_list;
typedef struct ast_record_field_ *          ast_rfield;
typedef struct ast_record_field_list_ *     ast_rfield_list;

typedef int ast_pos;

typedef enum {
    kind_op_plus,
    kind_op_minus,
    kind_op_times,
    kind_op_divide,
    kind_op_eq,
    kind_op_neq,
    kind_op_lt,
    kind_op_le,
    kind_op_gt,
    kind_op_ge,
} ast_op;

struct ast_dec_ {
    ast_pos pos;

    enum {
        kind_dec_var,
        kind_dec_type,
        kind_dec_func,
    } kind;

    union {
	    struct { symbol var, type; ast_exp init; bool escape; }                                 var;
	    struct { symbol type_s; ast_type type; }                                                type;
        struct { ast_pos pos; symbol func; ast_tfield_list params; symbol ret; ast_exp body; }  func;
	} u;
};

struct ast_exp_ {
    ast_pos pos;

    enum {
        kind_exp_var,
        kind_exp_nil,
        kind_exp_int,
        kind_exp_string,
        kind_exp_call,
	    kind_exp_op,
        kind_exp_record,
        kind_exp_seq,
        kind_exp_assign,
        kind_exp_if,
	    kind_exp_while,
        kind_exp_for,
        kind_exp_break,
        kind_exp_let,
        kind_exp_array,
    } kind;

    union {
        ast_var                                                     var;
        //                                                          nil;
	    int                                                         int_;
	    const char *                                                string_;
	    struct { symbol func; ast_exp_list args; }                  call;
	    struct { ast_op op; ast_exp left; ast_exp right; }          op;
	    struct { symbol record; ast_rfield_list members; }          record;
	    ast_exp_list                                                seq;
	    struct { ast_var var; ast_exp exp; }                        assign;
	    struct { ast_exp cond, then, else_; }                       if_;
	    struct { ast_exp cond, body;}                               while_;
	    struct { symbol var; ast_exp lo, hi, body; bool escape; }   for_;
	    //                                                          break;
	    struct { ast_dec_list decs; ast_exp_list body; }            let;
	    struct { symbol array; ast_exp size, init; }                array;
	} u;
};

struct ast_var_ {
    ast_pos pos;

    enum {
        kind_var,
        kind_var_slice,  // array slice
        kind_var_member, // record member
    } kind;

    union {
        symbol                                  var;
        struct { ast_var var; ast_exp exp; }    slice;
        struct { ast_var var; symbol member; }  member;
    } u;
};

struct ast_type_ {
    ast_pos pos;

    enum {
        kind_type_var,
        kind_type_array,
        kind_type_record,
    } kind;

	union {
        symbol          var;
		symbol          array;
		ast_tfield_list record;
	} u;
};

struct ast_dec_list_            { ast_dec       head; ast_dec_list      tail; };
struct ast_exp_list_            { ast_exp       head; ast_exp_list      tail; };
struct ast_type_field_list_     { ast_tfield    head; ast_tfield_list   tail; };
struct ast_record_field_list_   { ast_rfield    head; ast_rfield_list   tail; };
struct ast_type_field_          { symbol var, type; ast_pos pos; bool escape; };
struct ast_record_field_        { symbol var; ast_exp exp; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

ast_dec ast_mk_dec_var(ast_pos pos, symbol var, symbol type, ast_exp init);
ast_dec ast_mk_dec_type(symbol type_s, ast_type type);
ast_dec ast_mk_dec_func(ast_pos pos, symbol func, ast_tfield_list params, symbol ret, ast_exp body);

ast_exp ast_mk_exp_var(ast_pos pos, ast_var var);
ast_exp ast_mk_exp_nil(ast_pos pos);
ast_exp ast_mk_exp_int(ast_pos pos, int i);
ast_exp ast_mk_exp_string(ast_pos pos, const char *s);
ast_exp ast_mk_exp_call(ast_pos pos, symbol func, ast_exp_list args);
ast_exp ast_mk_exp_op(ast_pos pos, ast_op oper, ast_exp left, ast_exp right);
ast_exp ast_mk_exp_record(ast_pos pos, symbol record, ast_rfield_list members);
ast_exp ast_mk_exp_seq(ast_pos pos, ast_exp_list seq);
ast_exp ast_mk_exp_assign(ast_pos pos, ast_var var, ast_exp exp);
ast_exp ast_mk_exp_if(ast_pos pos, ast_exp test, ast_exp then, ast_exp elsee);
ast_exp ast_mk_exp_while(ast_pos pos, ast_exp test, ast_exp body);
ast_exp ast_mk_exp_for(ast_pos pos, symbol var, ast_exp lo, ast_exp hi, ast_exp body);
ast_exp ast_mk_exp_break(ast_pos pos);
ast_exp ast_mk_exp_let(ast_pos pos, ast_dec_list decs, ast_exp_list body);
ast_exp ast_mk_exp_array(ast_pos pos, symbol array, ast_exp size, ast_exp init);

ast_var ast_mk_var(ast_pos pos, symbol var);
ast_var ast_mk_var_slice(ast_pos pos, ast_var var, ast_exp exp);
ast_var ast_mk_var_member(ast_pos pos, ast_var var, symbol member);

ast_type ast_mk_type_var(ast_pos pos, symbol name);
ast_type ast_mk_type_array(ast_pos pos, symbol array);
ast_type ast_mk_type_record(ast_pos pos, ast_tfield_list record);

ast_dec_list ast_mk_dec_list(ast_dec head, ast_dec_list tail);
ast_exp_list ast_mk_exp_list(ast_exp head, ast_exp_list tail);
ast_tfield_list ast_mk_tfield_list(ast_tfield head, ast_tfield_list tail);
ast_rfield_list ast_mk_rfield_list(ast_rfield head, ast_rfield_list tail);
ast_tfield ast_mk_tfield(ast_pos pos, symbol var, symbol type);
ast_rfield ast_mk_rfield(symbol var, ast_exp exp);
