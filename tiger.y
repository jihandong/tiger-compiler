%{

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol.h"

/****************************************************************************
 * Public
 ****************************************************************************/

AST_exp AST_root;

extern FILE *yyin;
int yyparse(void);
int yylex(void);

void yyerror(char *s)
{
    fprintf(stderr, "Parse error: \"%s\"\n", s);
}

int parse(const char *filename)
{
    int ret;

    yyin = fopen(filename, "r");
    if (!yyin) {
        fprintf(stderr, "cannot open %s\n", filename);
        exit(1);
    }

    AST_root = NULL;
    ret = yyparse();

    fclose(yyin);
    return ret;
}

%}

%union
{
    int             pos;
    int             ival;
    const char *    sval;
    AST_dec         dec;
    AST_exp         exp;
    AST_var         var;
    AST_type        type;
    AST_dec_list    decs;
    AST_exp_list    exps;
    AST_para        para;
    AST_para_list   paras;
    AST_arg         arg;
    AST_arg_list    args;
}

%token <ival>   INT
%token <sval>   ID STRING
%token          COMMA COLON SEMICOLON LP RP LK RK LC RC
                DOT PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE AND OR ASSIGN
                ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF BREAK NIL
                FUNCTION VAR TYPE

%nonassoc   ASSIGN
%left       AND OR
%nonassoc   EQ NEQ LT LE GT GE
%left       PLUS MINUS
%left       TIMES DIVIDE
%nonassoc   UMINUS

%type <dec>     dec dec_var dec_type dec_func
%type <exp>     program exp exp_value exp_seq exp_op exp_call exp_create
                exp_assign exp_if exp_for exp_while exp_break exp_let
%type <var>     lvalue suffix
%type <type>    type
%type <decs>    decs
%type <exps>    sequence arguments
%type <para>    para
%type <paras>   paras
%type <arg>     arg
%type <args>    args

%start program

%%

program: exp { AST_root = $1; }

/****************************************************************************
 * declares
 ****************************************************************************/

decs
: /* epsilon */  { $$ = NULL; }
| dec decs       { $$ = AST_mk_dec_list($1, $2); }

dec
: dec_var  { $$ = $1; }
| dec_type { $$ = $1; }
| dec_func { $$ = $1; }

dec_var
: VAR ID ASSIGN exp {
    $$ = AST_mk_dec_var(0, SYM_declare($2), NULL, $4);
}
| VAR ID COLON ID ASSIGN exp {
    $$ = AST_mk_dec_var(0, SYM_declare($2), SYM_declare($4), $6);
}

dec_type
: TYPE ID EQ type { $$ = AST_mk_dec_type(SYM_declare($2), $4); }

dec_func
: FUNCTION ID LP RP EQ exp {
    $$ = AST_mk_dec_func(0, SYM_declare($2), NULL, NULL, $6); }
| FUNCTION ID LP RP COLON ID EQ exp {
    $$ = AST_mk_dec_func(0, SYM_declare($2), NULL, SYM_declare($6), $8);
}
| FUNCTION ID LP paras RP EQ exp {
    $$ = AST_mk_dec_func(0,  SYM_declare($2), $4, NULL, $7); }
| FUNCTION ID LP paras RP COLON ID EQ exp {
    $$ = AST_mk_dec_func(0, SYM_declare($2), $4, SYM_declare($7), $9);
}

/****************************************************************************
 * expressions
 ****************************************************************************/

exp
: exp_value  { $$ = $1; }
| exp_seq    { $$ = $1; }
| exp_op     { $$ = $1; }
| exp_call   { $$ = $1; }
| exp_create { $$ = $1; }
| exp_assign { $$ = $1; }
| exp_if     { $$ = $1; }
| exp_for    { $$ = $1; }
| exp_while  { $$ = $1; }
| exp_break  { $$ = $1; }
| exp_let    { $$ = $1; }
| error      { ; }

exp_value
: NIL       { $$ = AST_mk_exp_nil(0); }
| INT       { $$ = AST_mk_exp_int(0, $1); }
| STRING    { $$ = AST_mk_exp_str(0, $1); }
| lvalue    { $$ = AST_mk_exp_var(0, $1); }

exp_seq
: LP RP          { $$ = AST_mk_exp_seq(0, NULL); }
| LP sequence RP { $$ = AST_mk_exp_seq(0, $2); }

exp_op
: exp PLUS exp   { $$ = AST_mk_exp_op(0, AST_kind_op_plus,   $1, $3); }
| exp MINUS exp  { $$ = AST_mk_exp_op(0, AST_kind_op_minus,  $1, $3); }
| exp TIMES exp  { $$ = AST_mk_exp_op(0, AST_kind_op_times,  $1, $3); }
| exp DIVIDE exp { $$ = AST_mk_exp_op(0, AST_kind_op_divide, $1, $3); }
| exp EQ exp     { $$ = AST_mk_exp_op(0, AST_kind_op_eq,     $1, $3); }
| exp NEQ exp    { $$ = AST_mk_exp_op(0, AST_kind_op_neq,    $1, $3); }
| exp LT exp     { $$ = AST_mk_exp_op(0, AST_kind_op_lt,     $1, $3); }
| exp LE exp     { $$ = AST_mk_exp_op(0, AST_kind_op_le,     $1, $3); }
| exp GT exp     { $$ = AST_mk_exp_op(0, AST_kind_op_gt,     $1, $3); }
| exp GE exp     { $$ = AST_mk_exp_op(0, AST_kind_op_ge,     $1, $3); }
| MINUS exp %prec UMINUS {
    $$ = AST_mk_exp_op(0, AST_kind_op_minus, AST_mk_exp_int(0, 0), $2);
}

exp_call
: ID LP RP           { $$ = AST_mk_exp_call(0, SYM_declare($1), NULL); }
| ID LP arguments RP { $$ = AST_mk_exp_call(0, SYM_declare($1), $3); }

exp_create
: ID LC RC      { $$ = AST_mk_exp_record(0, SYM_declare($1), NULL); }
| ID LC args RC { $$ = AST_mk_exp_record(0, SYM_declare($1), $3); }
| ID LK exp RK OF exp { $$ = AST_mk_exp_array(0, SYM_declare($1), $3, $6); }

exp_assign
: lvalue ASSIGN exp { $$ = AST_mk_exp_assign(0, $1, $3); }

exp_if
: IF exp THEN exp ELSE exp { $$ = AST_mk_exp_if(0, $2, $4, $6); }
| IF exp THEN exp { $$ = AST_mk_exp_if(0, $2, $4, NULL); }
| exp AND exp     { $$ = AST_mk_exp_if(0, $1, $3, AST_mk_exp_int(0, 0)); }
| exp OR exp      { $$ = AST_mk_exp_if(0, $1, AST_mk_exp_int(0, 1), $3); }

exp_for
: FOR ID ASSIGN exp TO exp DO exp {
    $$ = AST_mk_exp_for(0, SYM_declare($2), $4, $6, $8);
}

exp_while
: WHILE exp DO exp { $$ = AST_mk_exp_while(0, $2, $4); }

exp_break
: BREAK { $$ = AST_mk_exp_break(0); }

exp_let
: LET decs IN END          { $$ = AST_mk_exp_let(0, $2, NULL); }
| LET decs IN sequence END { $$ = AST_mk_exp_let(0, $2, $4); }

/****************************************************************************
 * variables
 ****************************************************************************/

lvalue
: ID         { $$ = AST_mk_var_base(0, SYM_declare($1), NULL); }
| ID suffix  { $$ = AST_mk_var_base(0, SYM_declare($1), $2); }

suffix
: /* spsilon */    { $$ = NULL; }
| LK exp RK suffix { $$ = AST_mk_var_index(0, $2, $4); }
| DOT ID suffix    { $$ = AST_mk_var_field(0, SYM_declare($2), $3); }

/****************************************************************************
 * types
 ****************************************************************************/

type
: ID          { $$ = AST_mk_type_name(0, SYM_declare($1)); }
| ARRAY OF ID { $$ = AST_mk_type_array(0, SYM_declare($3)); }
| LC paras RC { $$ = AST_mk_type_record(0, $2); }
| LC RC       { $$ = AST_mk_type_record(0, NULL); }

/****************************************************************************
 * link list
 ****************************************************************************/

paras
: para             { $$ = AST_mk_para_list($1, NULL); }
| para COMMA paras { $$ = AST_mk_para_list($1, $3); }

para
: ID COLON ID { $$ = AST_mk_para(0, SYM_declare($1), SYM_declare($3)); }

args
: arg            { $$ = AST_mk_arg_list($1, NULL); }
| arg COMMA args { $$ = AST_mk_arg_list($1, $3); }

arg
: ID EQ exp { $$ = AST_mk_arg(SYM_declare($1), $3); }

sequence
: exp                    { $$ = AST_mk_exp_list($1, NULL); }
| exp SEMICOLON sequence { $$ = AST_mk_exp_list($1, $3); }

arguments
: exp                 { $$ = AST_mk_exp_list($1, NULL); }
| exp COMMA arguments { $$ = AST_mk_exp_list($1, $3); }

