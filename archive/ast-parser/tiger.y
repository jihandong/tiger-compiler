%{

#include <stdio.h>
#include "ast.h"

#define AST_POS 0

int yylex(void); /* function prototype */

void yyerror(char *s)
{
    fprintf(stderr, "Parse error: \"%s\"\n", s);
}

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

/****************************************************************************
 * Expressions
 ****************************************************************************/

program: exp { $$ = $1; }

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

left_value
: ID                            { $$ = mk_ast_var_trival(AST_POS, $1); }
| left_value DOT ID             { $$ = mk_ast_var_field(AST_POS, $1, $3; }
| left_value LBRACK exp RBRACK  { $$ = mk_ast_var_subscript(AST_POS, $1, $3); }

exp_value
: NIL           { $$ = mk_ast_exp_nil(AST_POS); }
| INT           { $$ = mk_ast_exp_int(AST_POS, $1); }
| STRING        { $$ = mk_ast_exp_string(AST_POS, $1); }
| left_value    { $$ = mk_ast_exp_var(AST_POS, $1); }

exp_sequence
: LPAREN sequence_fields RPAREN { $$ = $$2; }

exp_op
: exp PLUS exp              { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_plus,   $1, $3); }
| exp MINUS exp             { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_minus,  $1, $3); }
| exp TIMES exp             { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_times,  $1, $3); }
| exp DIVIDE exp            { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_divide, $1, $3); }
| exp EQ exp                { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_eq,     $1, $3); }
| exp NEQ exp               { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_neq,    $1, $3); }
| exp LT exp                { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_lt,     $1, $3); }
| exp LE exp                { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_le,     $1, $3); }
| exp GT exp                { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_gt,     $1, $3); }
| exp GE exp                { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_ge,     $1, $3); }
| exp AND exp               { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_and,    $1, $3); }
| exp OR exp                { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_or,     $1, $3); }
| MINUS exp %prec UMINUS    { $$ = mk_ast_exp_op(AST_POS, kind_ast_op_uminus, NULL, $3); }

exp_call
: ID LPAREN RPAREN                  { $$ = mk_ast_exp_call(AST_POS, $1, NULL); }
| ID LPAREN argument_fields RPAREN  { $$ = mk_ast_exp_call(AST_POS, $1, $3); }

exp_define
: ID LBRACE RBRACE                  { $$ = mk_ast_exp_record(AST_POS, $1, NULL); }
| ID LBRACE record_fields RBRACE    { $$ = mk_ast_exp_record(AST_POS, $1, $3); }
| ID LBRACK exp RBRACK OF exp       { $$ = mk_ast_exp_array(AST_POS, $1, $3, $6); }

exp_assign
: left_value ASSIGN exp { $$ = mk_ast_exp_assign(AST_POS, $1, $2); }

exp_if
: IF exp THEN exp           { $$ = mk_ast_exp_if(AST_POS, $2, $4, NULL); }
| IF exp THEN exp ELSE exp  { $$ = mk_ast_exp_if(AST_POS, $2, $4, $6); }

exp_for
: FOR ID ASSIGN exp TO exp DO exp { $$ = mk_ast_exp_for(AST_POS, $2, $4, $6, $8); }

exp_while
: WHILE exp DO exp { $$ = mk_ast_exp_while(AST_POS, $2, $4); }

exp_break
: BREAK { $$ = mk_ast_exp_break(AST_POS); }

exp_let
: LET declares IN END                   { $$ = mk_ast_exp_let(AST_POS, $2, NULL); }
| LET declares IN sequence_fields END   { $$ = mk_ast_exp_let(AST_POS, $2, $4); }

/****************************************************************************
 * Declerations
 ****************************************************************************/

declares
: /* epsilon */     { $$ = NULL; }
| declare declares  { $$ = mk_ast_dec_list($1, $2); }

declare
: type_declare      { ; }
| variable_declare  { ; }
| function_declare  { ; }

type_declare
: TYPE ID EQ type   { $$ = mk_ast_dec_type }

type
: ID                        { ; }
| LBRACE type_fields RBRACE { ; }
| ARRAY OF ID               { ; }

variable_declare
: VAR ID ASSIGN exp             { ; }
| VAR ID COLON ID ASSIGN exp    { ; }

function_declare
: FUNCTION ID LPAREN type_fields RPAREN EQ exp          { ; }
| FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ exp { ; }

/****************************************************************************
 * Fields
 ****************************************************************************/

type_fields
: /* epsilon */                 { $$ = NULL; }
| ID COLON ID                   { }
| ID COLON ID COMMA type_fields { }

sequence_fields
: exp                           { $$ = mk_ast_exp_list($1, NULL); }
| exp SEMICOLON sequence_fields { $$ = mk_ast_exp_list($1, $3); }

argument_fields
: exp                       { $$ = mk_ast_arg_list($1, NULL); }
| exp COMMA argument_fields { $$ = mk_ast_arg_list($1, $3); }

record_fields
: ID EQ exp                     { $$ = mk_ast_record_list(mk_ast_record($1, $3), NULL); }
| ID EQ exp COMMA record_fields { $$ = mk_ast_record_list(mk_ast_record($1, $3), $5); }
