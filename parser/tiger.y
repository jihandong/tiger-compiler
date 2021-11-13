%{

#include <stdio.h>

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

program: exp

exp
: exp_value         { printf("expression: value\n"); }
| exp_sequence      { printf("expression: sequence\n"); }
| exp_op            { printf("expression: op\n"); }
| exp_call          { printf("expression: call\n"); }
| exp_define        { printf("expression: define\n"); }
| exp_assign        { printf("expression: assign\n"); }
| exp_if            { printf("expression: if\n"); }
| exp_for           { printf("expression: for\n"); }
| exp_while         { printf("expression: while\n"); }
| exp_break         { printf("expression: break\n"); }
| exp_let           { printf("expression: let\n"); }

left_value
: ID                            { printf("left value: identifier\n"); }
| left_value DOT ID             { printf("left value: record member\n"); }
| left_value LBRACK ID RBRACK   { printf("left value: array member\n"); }

exp_value
: NIL           { printf("expression value: nil\n"); }
| INT           { printf("expression value: integer\n"); }
| STRING        { printf("expression value: string\n"); }
| left_value    { printf("expression value: left value\n"); }

exp_sequence
: LPAREN sequence_fields RPAREN

exp_op
: exp PLUS exp              { printf("operand1 +  operand2\n"); }
| exp MINUS exp             { printf("operand1 -  operand2\n"); }
| exp TIMES exp             { printf("operand1 *  operand2\n"); }
| exp DIVIDE exp            { printf("operand1 /  operand2\n"); }
| exp EQ exp                { printf("operand1 =  operand2\n"); }
| exp NEQ exp               { printf("operand1 <> operand2\n"); }
| exp LT exp                { printf("operand1 <  operand2\n"); }
| exp LE exp                { printf("operand1 <= operand2\n"); }
| exp GT exp                { printf("operand1 >  operand2\n"); }
| exp GE exp                { printf("operand1 >= operand2\n"); }
| exp AND exp               { printf("operand1 &  operand2\n"); }
| exp OR exp                { printf("operand1 |  operand2\n"); }
| MINUS exp %prec UMINUS    { printf("- operand\n"); }

exp_call
: ID LPAREN argument_fields RPAREN

exp_define
: ID LBRACE record_fields RBRACE
| ID LBRACK exp RBRACK OF exp

exp_assign
: left_value ASSIGN exp

exp_if
: IF exp THEN exp           { printf("if then\n"); }
| IF exp THEN exp ELSE exp  { printf("if then else\n"); }

exp_for
: FOR ID ASSIGN exp TO exp DO exp

exp_while
: WHILE exp DO exp

exp_break
: BREAK

exp_let
: LET declares IN exp_sequence END

/****************************************************************************
 * Declerations
 ****************************************************************************/

declares: /* epsilon */
        | declare declares

declare: type_declare
       | variable_declare
       | function_declare

type_declare: TYPE ID EQ type

type: ID
    | LBRACE type_fields RBRACE
    | ARRAY OF ID

variable_declare: VAR ID ASSIGN exp
                | VAR ID COLON ID ASSIGN exp

function_declare: FUNCTION ID LPAREN type_fields RPAREN EQ exp
                | FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ exp

/****************************************************************************
 * Fields
 ****************************************************************************/

type_fields: /* epsilon */
           | ID COLON ID
           | ID COLON ID COMMA type_fields

sequence_fields: /* epsilon */
               | exp
               | exp SEMICOLON sequence_fields

argument_fields: /* epsilon */
               | exp
               | exp COMMA argument_fields

record_fields: /* epsilon */
             | ID EQ exp
             | ID EQ exp COMMA record_fields
