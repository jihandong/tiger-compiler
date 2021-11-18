/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * 
 ****************************************************************************/

typedef struct ast_var_ *               ast_var;
typedef struct ast_exp_ *               ast_exp;
typedef struct ast_dec_ *               ast_dec;
typedef struct ast_type_ *              ast_type;
typedef struct ast_dec_list_ *          ast_dec_list;
typedef struct ast_exp_list_ *          ast_exp_list;
typedef struct ast_field_ *             ast_field;
typedef struct ast_field_list_ *        ast_field_list;
typedef struct ast_function_ *          ast_function;
typedef struct ast_function_list_ *     ast_function_list;
typedef struct ast_type_id_ *           ast_type_id;
typedef struct ast_type_id_list_ *      ast_type_id_list;
typedef struct ast_record_field_ *      ast_record_field;
typedef struct ast_record_field_list_ * ast_record_field_list;

typedef enum {
    ast_op_plus, ast_op_minus, ast_op_times, ast_op_divide,
    ast_op_eq, ast_op_neq, ast_op_lt, ast_op_le, ast_op_gt, ast_op_ge
} ast_op;

typedef int ast_pos;

/****************************************************************************
 * 
 ****************************************************************************/

struct ast_var_ {
    ast_pos pos;

    enum {
        ast_var_trival,
        ast_var_field,
        ast_var_subscript,
    } kind;

    union {
        symbol                                  trival;
        struct { ast_var var; symbol  sym; }    field;
        struct { ast_var var; ast_exp exp; }    subscript;
    } u;
};

struct ast_exp_ {
    ast_pos pos;

    enum {
        ast_exp_var,
        ast_exp_nil,
        ast_exp_int,
        ast_exp_string,
        ast_exp_call,
	    ast_exp_op,
        ast_exp_record,
        ast_exp_seq,
        ast_exp_assign,
        ast_exp_if,
	    ast_exp_while,
        ast_exp_for,
        ast_exp_break,
        ast_exp_let,
        ast_exp_array,
    } kind;

    union {
        ast_var                                                     var;
        //                                                          nil;
	    int                                                         integer_;
	    const char *                                                string_;
	    struct { symbol func; ast_exp_list args; }                  call;
	    struct { ast_op op; ast_exp left; ast_exp right; }          op;
	    struct { symbol type; ast_record_field_list fields; }       record;
	    ast_exp_list                                                seq;
	    struct { ast_var var; ast_exp exp; }                        assign;
	    struct { ast_exp cond, then, else_; }                       if_;
	    struct { ast_exp cond, body;}                               while_;
	    struct { symbol var; ast_exp lo, hi, body; bool escape; }   for_;
	    //                                                          break;
	    struct { ast_dec_list decs; ast_exp body; }                 let;
	    struct { symbol type; ast_exp size, init; }                 array;
	} u;
};

struct ast_dec_ {
    ast_pos pos;

    enum {
        ast_dec_function,
        ast_dec_type,
        ast_dec_var,
    } kind;

    union {
        ast_function_list                                           function;
	    ast_type_id_list                                                type;
	    struct { symbol var; symbol typ; ast_exp init; bool escape; }   var;
	} u;
};

struct ast_type_ {
    ast_pos pos;

    enum {
        ast_nameTy,
        ast_arrayTy,
        ast_recordTy,
    } kind;

	union {
        symbol          name;
		symbol          array;
		ast_field_list  record;
	} u;
};

struct ast_field_       { symbol name, type; ast_pos pos; bool escape; };
struct ast_field_list_  { ast_field head; ast_field_list tail; };
struct ast_exp_list_ { ast_exp head; ast_exp_list tail; };
struct ast_function_ { ast_pos pos; symbol name; ast_field_list params; symbol result; ast_exp body; };
struct ast_function_list_ { ast_function head; ast_function_list tail; };
struct ast_dec_list_ { ast_dec head; ast_dec_list tail; };
struct ast_type_id_ { symbol name; ast_type type; };
struct ast_type_id_list_ { ast_type_id head; ast_type_id_list tail; };
struct ast_record_field_ { symbol name; ast_exp exp; };
struct ast_record_field_list_ { ast_record_field head; ast_record_field_list tail; };

/****************************************************************************
 * 
 ****************************************************************************/