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

A_exp A_root;

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

    A_root = NULL;
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
    A_dec           dec;
    A_exp           exp;
    A_var           var;
    A_type          type;
    A_dec_list      decs;
    A_exp_list      exps;
    A_para          para;
    A_para_list     paras;
    A_arg           arg;
    A_arg_list      args;
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

program: exp { A_root = $1; }

/****************************************************************************
 * declares
 ****************************************************************************/

decs
: /* epsilon */  { $$ = NULL; }
| dec decs       { $$ = A_mk_dec_list($1, $2); }

dec
: dec_var  { $$ = $1; }
| dec_type { $$ = $1; }
| dec_func { $$ = $1; }

dec_var
: VAR ID ASSIGN exp {
    $$ = A_mk_dec_var(0, S_mk_symbol($2), NULL, $4);
}
| VAR ID COLON ID ASSIGN exp {
    $$ = A_mk_dec_var(0, S_mk_symbol($2), S_mk_symbol($4), $6);
}

dec_type
: TYPE ID EQ type { $$ = A_mk_dec_type(S_mk_symbol($2), $4); }

dec_func
: FUNCTION ID LP RP EQ exp {
    $$ = A_mk_dec_func(0, S_mk_symbol($2), NULL, NULL, $6); }
| FUNCTION ID LP RP COLON ID EQ exp {
    $$ = A_mk_dec_func(0, S_mk_symbol($2), NULL, S_mk_symbol($6), $8);
}
| FUNCTION ID LP paras RP EQ exp {
    $$ = A_mk_dec_func(0,  S_mk_symbol($2), $4, NULL, $7); }
| FUNCTION ID LP paras RP COLON ID EQ exp {
    $$ = A_mk_dec_func(0, S_mk_symbol($2), $4, S_mk_symbol($7), $9);
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
: NIL       { $$ = A_mk_exp_nil(0); }
| INT       { $$ = A_mk_exp_int(0, $1); }
| STRING    { $$ = A_mk_exp_str(0, $1); }
| lvalue    { $$ = A_mk_exp_var(0, $1); }

exp_seq
: LP RP          { $$ = A_mk_exp_seq(0, NULL); }
| LP sequence RP { $$ = A_mk_exp_seq(0, $2); }

exp_op
: exp PLUS exp           { $$ = A_mk_exp_op(0, A_kind_op_plus,   $1, $3); }
| exp MINUS exp          { $$ = A_mk_exp_op(0, A_kind_op_minus,  $1, $3); }
| exp TIMES exp          { $$ = A_mk_exp_op(0, A_kind_op_times,  $1, $3); }
| exp DIVIDE exp         { $$ = A_mk_exp_op(0, A_kind_op_divide, $1, $3); }
| exp EQ exp             { $$ = A_mk_exp_op(0, A_kind_op_eq,     $1, $3); }
| exp NEQ exp            { $$ = A_mk_exp_op(0, A_kind_op_neq,    $1, $3); }
| exp LT exp             { $$ = A_mk_exp_op(0, A_kind_op_lt,     $1, $3); }
| exp LE exp             { $$ = A_mk_exp_op(0, A_kind_op_le,     $1, $3); }
| exp GT exp             { $$ = A_mk_exp_op(0, A_kind_op_gt,     $1, $3); }
| exp GE exp             { $$ = A_mk_exp_op(0, A_kind_op_ge,     $1, $3); }
| MINUS exp %prec UMINUS {
    $$ = A_mk_exp_op(0, A_kind_op_minus, A_mk_exp_int(0, 0), $2);
}

exp_call
: ID LP RP           { $$ = A_mk_exp_call(0, S_mk_symbol($1), NULL); }
| ID LP arguments RP { $$ = A_mk_exp_call(0, S_mk_symbol($1), $3); }

exp_create
: ID LC RC            { $$ = A_mk_exp_record(0, S_mk_symbol($1), NULL); }
| ID LC args RC       { $$ = A_mk_exp_record(0, S_mk_symbol($1), $3); }
| ID LK exp RK OF exp { $$ = A_mk_exp_array(0, S_mk_symbol($1), $3, $6); }

exp_assign
: lvalue ASSIGN exp { $$ = A_mk_exp_assign(0, $1, $3); }

exp_if
: IF exp THEN exp ELSE exp  { $$ = A_mk_exp_if(0, $2, $4, $6); }
| IF exp THEN exp           { $$ = A_mk_exp_if(0, $2, $4, NULL); }
| exp AND exp               { $$ = A_mk_exp_if(0, $1, $3, A_mk_exp_int(0, 0)); }
| exp OR exp                { $$ = A_mk_exp_if(0, $1, A_mk_exp_int(0, 1), $3); }

exp_for
: FOR ID ASSIGN exp TO exp DO exp {
    $$ = A_mk_exp_for(0, S_mk_symbol($2), $4, $6, $8);
}

exp_while
: WHILE exp DO exp { $$ = A_mk_exp_while(0, $2, $4); }

exp_break
: BREAK { $$ = A_mk_exp_break(0); }

exp_let
: LET decs IN END          { $$ = A_mk_exp_let(0, $2, NULL); }
| LET decs IN sequence END { $$ = A_mk_exp_let(0, $2, $4); }

/****************************************************************************
 * variables
 ****************************************************************************/

lvalue
: ID         { $$ = A_mk_var_base(0, S_mk_symbol($1), NULL); }
| ID suffix  { $$ = A_mk_var_base(0, S_mk_symbol($1), $2); }

suffix
: /* spsilon */    { $$ = NULL; }
| LK exp RK suffix { $$ = A_mk_var_index(0, $2, $4); }
| DOT ID suffix    { $$ = A_mk_var_field(0, S_mk_symbol($2), $3); }

/****************************************************************************
 * types
 ****************************************************************************/

type
: ID          { $$ = A_mk_type_name(0, S_mk_symbol($1)); }
| ARRAY OF ID { $$ = A_mk_type_array(0, S_mk_symbol($3)); }
| LC paras RC { $$ = A_mk_type_record(0, $2); }
| LC RC       { $$ = A_mk_type_record(0, NULL); }

/****************************************************************************
 * link list
 ****************************************************************************/

paras
: para             { $$ = A_mk_para_list($1, NULL); }
| para COMMA paras { $$ = A_mk_para_list($1, $3); }

para
: ID COLON ID { $$ = A_mk_para(0, S_mk_symbol($1), S_mk_symbol($3)); }

args
: arg            { $$ = A_mk_arg_list($1, NULL); }
| arg COMMA args { $$ = A_mk_arg_list($1, $3); }

arg
: ID EQ exp { $$ = A_mk_arg(S_mk_symbol($1), $3); }

sequence
: exp                    { $$ = A_mk_exp_list($1, NULL); }
| exp SEMICOLON sequence { $$ = A_mk_exp_list($1, $3); }

arguments
: exp                 { $$ = A_mk_exp_list($1, NULL); }
| exp COMMA arguments { $$ = A_mk_exp_list($1, $3); }

