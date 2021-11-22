/****************************************************************************
 * Include Files
 ****************************************************************************/

#include "symbol.h"

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

typedef struct ast_var_ *               ast_var;
typedef struct ast_exp_ *               ast_exp;
typedef struct ast_dec_ *               ast_dec;
typedef struct ast_type_ *              ast_type;
typedef struct ast_exp_list_ *          ast_exp_list;
typedef struct ast_dec_list_ *          ast_dec_list;
typedef struct ast_func_dec_ *          ast_func_dec;
typedef struct ast_func_dec_list_ *     ast_func_dec_list;
typedef struct ast_type_dec_ *          ast_type_dec;
typedef struct ast_type_dec_list_ *     ast_type_dec_list;
typedef struct ast_field_ *             ast_field;
typedef struct ast_field_list_ *        ast_field_list;
typedef struct ast_efield_ *            ast_efield;
typedef struct ast_efield_list_ *       ast_efield_list;

typedef enum {
    kind_ast_op_plus,   kind_ast_op_minus,
    kind_ast_op_times,  kind_ast_op_divide,
    kind_ast_op_eq,     kind_ast_op_neq,
    kind_ast_op_lt,     kind_ast_op_le,
    kind_ast_op_gt,     kind_ast_op_ge,
    kind_ast_op_uminus,
} ast_op;

typedef int ast_pos;

struct ast_var_ {
    ast_pos pos;

    enum {
        kind_ast_var_trival,
        kind_ast_var_field,
        kind_ast_var_subscript,
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
        kind_ast_exp_var,
        kind_ast_exp_nil,
        kind_ast_exp_int,
        kind_ast_exp_string,
        kind_ast_exp_call,
	    kind_ast_exp_op,
        kind_ast_exp_record,
        kind_ast_exp_seq,
        kind_ast_exp_assign,
        kind_ast_exp_if,
	    kind_ast_exp_while,
        kind_ast_exp_for,
        kind_ast_exp_break,
        kind_ast_exp_let,
        kind_ast_exp_array,
    } kind;

    union {
        ast_var                                                     var;
        //                                                          nil;
	    int                                                         int_;
	    const char *                                                string_;
	    struct { symbol func; ast_exp_list args; }                  call;
	    struct { ast_op op; ast_exp left; ast_exp right; }          op;
	    struct { symbol name; ast_efield_list fields; }             record;
	    ast_exp_list                                                seq;
	    struct { ast_var var; ast_exp exp; }                        assign;
	    struct { ast_exp cond, then, else_; }                       if_;
	    struct { ast_exp cond, body;}                               while_;
	    struct { symbol var; ast_exp lo, hi, body; bool escape; }   for_;
	    //                                                          break;
	    struct { ast_dec_list decs; ast_exp body; }                 let;
	    struct { symbol name; ast_exp size, init; }                 array;
	} u;
};

struct ast_dec_ {
    ast_pos pos;

    enum {
        kind_ast_dec_function,
        kind_ast_dec_type,
        kind_ast_dec_var,
    } kind;

    union {
        ast_func_dec_list                                           function;
	    ast_type_dec_list                                               type;
	    struct { symbol var; symbol name; ast_exp init; bool escape; }   var;
	} u;
};

struct ast_type_ {
    ast_pos pos;

    enum {
        kind_ast_type_name,
        kind_ast_type_array,
        kind_ast_type_record,
    } kind;

	union {
        symbol          name;
		symbol          array;
		ast_field_list record;
	} u;
};

/****************************************************************************
 * Link List Definitions
 ****************************************************************************/

struct ast_func_dec_ { 
    ast_pos pos; symbol name; ast_field_list params; symbol result; ast_exp body;
};
struct ast_type_dec_ { symbol name; ast_type type; };
struct ast_field_    { symbol name, type_name; ast_pos pos; bool escape; };
struct ast_efield_   { symbol name; ast_exp exp; };

struct ast_exp_list_        { ast_exp       head; ast_exp_list      tail; };
struct ast_dec_list_        { ast_dec       head; ast_dec_list      tail; };
struct ast_func_dec_list_   { ast_func_dec  head; ast_func_dec_list tail; };
struct ast_type_dec_list_   { ast_type_dec  head; ast_type_dec_list tail; };
struct ast_field_list_      { ast_field     head; ast_field_list    tail; };
struct ast_efield_list_     { ast_efield    head; ast_efield_list   tail; };

/****************************************************************************
 * Public Functions
 ****************************************************************************/

ast_var mk_ast_var_trival(ast_pos pos, symbol sym);
ast_var mk_ast_var_field(ast_pos pos, ast_var var, symbol sym);
ast_var mk_ast_var_subscript(ast_pos pos, ast_var var, ast_exp exp);

ast_exp mk_ast_exp_var(ast_pos pos, ast_var var);
ast_exp mk_ast_exp_nil(ast_pos pos);
ast_exp mk_ast_exp_int(ast_pos pos, int i);
ast_exp mk_ast_exp_string(ast_pos pos, const char *s);
ast_exp mk_ast_exp_call(ast_pos pos, symbol func, ast_exp_list args);
ast_exp mk_ast_exp_op(ast_pos pos, ast_op oper, ast_exp left, ast_exp right);
ast_exp mk_ast_exp_record(ast_pos pos, symbol name, ast_efield_list fields);
ast_exp mk_ast_exp_seq(ast_pos pos, ast_exp_list seq);
ast_exp mk_ast_exp_assign(ast_pos pos, ast_var var, ast_exp exp);
ast_exp mk_ast_exp_if(ast_pos pos, ast_exp test, ast_exp then, ast_exp elsee);
ast_exp mk_ast_exp_while(ast_pos pos, ast_exp test, ast_exp body);
ast_exp mk_ast_exp_for(ast_pos pos, symbol var, ast_exp lo, ast_exp hi, ast_exp body);
ast_exp mk_ast_exp_break(ast_pos pos);
ast_exp mk_ast_exp_let(ast_pos pos, ast_dec_list decs, ast_exp body);
ast_exp mk_ast_exp_array(ast_pos pos, symbol typ, ast_exp size, ast_exp init);

ast_dec mk_ast_dec_function(ast_pos pos, ast_func_dec_list function);
ast_dec mk_ast_dec_type(ast_pos pos, ast_type_dec_list type);
ast_dec mk_ast_dec_var(ast_pos pos, symbol var, symbol type_name, ast_exp init);

ast_type mk_ast_type_name(ast_pos pos, symbol name);
ast_type mk_ast_type_array(ast_pos pos, symbol array);
ast_type mk_ast_type_record(ast_pos pos, ast_field_list record);

ast_func_dec mk_ast_func_dec(ast_pos pos, symbol name, ast_field_list params,
                             symbol result, ast_exp body);
ast_type_dec mk_ast_type_dec(symbol name, ast_type type);
ast_field mk_ast_field(ast_pos pos, symbol name, symbol type_name);
ast_efield mk_ast_efield(symbol name, ast_exp exp);
ast_exp_list mk_ast_exp_list(ast_exp head, ast_exp_list tail);
ast_dec_list mk_ast_dec_list(ast_dec head, ast_dec_list tail);
ast_func_dec_list mk_ast_func_dec_list(ast_func_dec head, ast_func_dec_list tail);
ast_type_dec_list mk_ast_type_dec_list(ast_type_dec head, ast_type_dec_list tail);
ast_field_list mk_ast_arg_list(ast_field head, ast_field_list tail);
ast_efield_list mk_ast_efield_list(ast_efield head, ast_efield_list tail);
