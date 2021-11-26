%{

/********************************************************************************
 * Includes
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

/********************************************************************************
 * External
 ********************************************************************************/

extern FILE *yyin;
int yyparse(void);
int yylex(void);

/********************************************************************************
 * Public
 ********************************************************************************/

ast_exp ast_root;

void yyerror(char *s)
{
    fprintf(stderr, "Parse error: \"%s\"\n", s);
}

ast_exp parse(const char *filename)
{
    yyin = fopen(filename, "r");
    if (!yyin) {
        fprintf(stderr, "cannot open %s\n", filename);
        exit(1);
    }

    if (yyparse())
        return NULL;

    return ast_root;
}

/********************************************************************************
 * End
 ********************************************************************************/

%}

%union
{
	int         pos;
	int         ival;
	const char *sval;
}

%token <ival> INT
%token <sval> ID STRING
%token
COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE DOT
PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE AND OR ASSIGN
ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF BREAK NIL FUNCTION VAR TYPE

%nonassoc ASSIGN
%left AND OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS
%start program

%%

program: exp { ast_root = $1; }

/****************************************************************************
 * declares
 ****************************************************************************/

decs
: /* epsilon */ { $$ = NULL; }
| dec decs      { $$ = mk_ast_dec_list($1, $2); }

dec
: dec_var   { $$ = $1; }
| dec_type  { $$ = $1; }
| dec_func  { $$ = $1; }

dec_var
: VAR ID ASSIGN exp             { $$ = mk_ast_dec_var(0, $2, NULL, $4); }
| VAR ID COLON ID ASSIGN exp    { $$ = mk_ast_dec_var(0, $2, $4, $6); }

dec_type
: TYPE ID EQ type   { $$ = mk_ast_type_dec($2, $4); }

dec_func
: FUNCTION ID LPAREN type_fields RPAREN EQ exp          { $$ = mk_ast_func_dec(0, $2, $4, NULL, $7); }
| FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ exp { $$ = mk_ast_func_dec(0, $2, $4, $7, $9); }

/****************************************************************************
 * expressions
 ****************************************************************************/

exp
: exp_value     { $$ = $1; }
| exp_sequence  { $$ = $1; }
| exp_op        { $$ = $1; }
| exp_call      { $$ = $1; }
| exp_define    { $$ = $1; }
| exp_assign    { $$ = $1; }
| exp_if        { $$ = $1; }
| exp_for       { $$ = $1; }
| exp_while     { $$ = $1; }
| exp_break     { $$ = $1; }
| exp_let       { $$ = $1; }
| error

exp_value
: NIL       { $$ = mk_ast_exp_nil(0); }
| INT       { $$ = mk_ast_exp_int(0, $1); }
| STRING    { $$ = mk_ast_exp_string(0, $1); }
| lvalue    { $$ = mk_ast_exp_var(0, $1); }

exp_sequence
: LPAREN sequence_fields RPAREN { $$ = mk_ast_exp_seq(0, $2); }

exp_op
: exp PLUS exp              { $$ = mk_ast_exp_op(0, kind_op_plus,   $1, $3); }
| exp MINUS exp             { $$ = mk_ast_exp_op(0, kind_op_minus,  $1, $3); }
| exp TIMES exp             { $$ = mk_ast_exp_op(0, kind_op_times,  $1, $3); }
| exp DIVIDE exp            { $$ = mk_ast_exp_op(0, kind_op_divide, $1, $3); }
| exp EQ exp                { $$ = mk_ast_exp_op(0, kind_op_eq,     $1, $3); }
| exp NEQ exp               { $$ = mk_ast_exp_op(0, kind_op_neq,    $1, $3); }
| exp LT exp                { $$ = mk_ast_exp_op(0, kind_op_lt,     $1, $3); }
| exp LE exp                { $$ = mk_ast_exp_op(0, kind_op_le,     $1, $3); }
| exp GT exp                { $$ = mk_ast_exp_op(0, kind_op_gt,     $1, $3); }
| exp GE exp                { $$ = mk_ast_exp_op(0, kind_op_ge,     $1, $3); }
| exp AND exp               { $$ = mk_ast_exp_op(0, kind_op_and,    $1, $3); }
| exp OR exp                { $$ = mk_ast_exp_op(0, kind_op_or,     $1, $3); }
| MINUS exp %prec UMINUS    { $$ = mk_ast_exp_op(0, kind_op_uminus, NULL, $3); }

exp_call
: ID LPAREN RPAREN                  { $$ = mk_ast_exp_call(0, $1, NULL); }
| ID LPAREN argument_fields RPAREN  { $$ = mk_ast_exp_call(0, $1, $3); }

exp_define
: ID LBRACE RBRACE                  { $$ = mk_ast_exp_record(0, $1, NULL); }
| ID LBRACE record_fields RBRACE    { $$ = mk_ast_exp_record(0, $1, $3); }
| ID LBRACK exp RBRACK OF exp       { $$ = mk_ast_exp_array(0, $1, $3, $6); }

exp_assign
: lvalue ASSIGN exp { $$ = mk_ast_exp_assign(0, $1, $2); }

exp_if
: IF exp THEN exp           { $$ = mk_ast_exp_if(0, $2, $4, NULL); }
| IF exp THEN exp ELSE exp  { $$ = mk_ast_exp_if(0, $2, $4, $6); }

exp_for
: FOR ID ASSIGN exp TO exp DO exp { $$ = mk_ast_exp_for(0, $2, $4, $6, $8); }

exp_while
: WHILE exp DO exp { $$ = mk_ast_exp_while(0, $2, $4); }

exp_break
: BREAK { $$ = mk_ast_exp_break(0); }

exp_let
: LET decs IN END                   { $$ = mk_ast_exp_let(0, $2, NULL); }
| LET decs IN sequence_fields END   { $$ = mk_ast_exp_let(0, $2, $4); }

/****************************************************************************
 * variables
 ****************************************************************************/

lvalue
: ID                        { $$ = mk_ast_var_trival(0, $1); }
| lvalue DOT ID             { $$ = mk_ast_var_field(0, $1, $3); }
| lvalue LBRACK exp RBRACK  { $$ = mk_ast_var_subscript(0, $1, $3); }

/****************************************************************************
 * types
 ****************************************************************************/

type
: ID                        { $$ = mk_ast_type_var(0, $1); }
| ARRAY OF ID               { $$ = mk_ast_type_array(0, $3); }
| LBRACE type_fields RBRACE { $$ = mk_ast_type_record(0, $2); }

/****************************************************************************
 * link list
 ****************************************************************************/

type_fields
: /* epsilon */                 { $$ = NULL; }
| ID COLON ID                   { $$ = mk_ast_tfield(0, $1, $3); }
| ID COLON ID COMMA type_fields { $$ = mk_ast_tfield_list(mk_ast_field(0, $1, $3), $5); }

record_fields
: ID EQ exp                     { $$ = mk_ast_rfield($1, $3); }
| ID EQ exp COMMA record_fields { $$ = mk_ast_rfield_list(mk_ast_efield($1, $3), $5); }

sequence_fields
: exp                           { $$ = $1; }
| exp SEMICOLON sequence_fields { $$ = mk_ast_exp_list($1, $3); }

argument_fields
: exp                       { $$ = $1; }
| exp COMMA argument_fields { $$ = mk_ast_exp_list($1, $3); }
