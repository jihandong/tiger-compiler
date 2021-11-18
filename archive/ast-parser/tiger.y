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
: exp_value     
| exp_sequence  
| exp_op        
| exp_call      
| exp_define    
| exp_assign    
| exp_if        
| exp_for       
| exp_while     
| exp_break     
| exp_let       
| error

left_value
: ID left_value_right

left_value_right
: /* epsilon */
| DOT ID left_value_right
| LBRACK exp RBRACK left_value_right


exp_value
: NIL           
| INT           
| STRING        
| left_value    

exp_sequence
: LPAREN sequence_fields RPAREN

exp_op
: exp PLUS exp              { $$ = $1 +  $3; }
| exp MINUS exp             { $$ = $1 -  $3; }
| exp TIMES exp             { $$ = $1 *  $3; }
| exp DIVIDE exp            { $$ = $1 /  $3; }
| exp EQ exp                { $$ = $1 == $3; }
| exp NEQ exp               { $$ = $1 != $3; }
| exp LT exp                { $$ = $1 <  $3; }
| exp LE exp                { $$ = $1 <= $3; }
| exp GT exp                { $$ = $1 >  $3; }
| exp GE exp                { $$ = $1 <= $3; }
| exp AND exp               { $$ = $1 && $3; }
| exp OR exp                { $$ = $1 || $3; }
| MINUS exp %prec UMINUS    { $$ = - $2; }

exp_call
: ID LPAREN RPAREN
| ID LPAREN argument_fields RPAREN

exp_define
: ID LBRACE RBRACE                  
| ID LBRACE record_fields RBRACE    
| ID LBRACK exp RBRACK OF exp       

exp_assign
: left_value ASSIGN exp

exp_if
: IF exp THEN exp           
| IF exp THEN exp ELSE exp  

exp_for
: FOR ID ASSIGN exp TO exp DO exp

exp_while
: WHILE exp DO exp

exp_break
: BREAK

exp_let
: LET declares IN END                   
| LET declares IN sequence_fields END   

/****************************************************************************
 * Declerations
 ****************************************************************************/

declares
: /* epsilon */
| declare declares

declare
: type_declare
| variable_declare
| function_declare

type_declare
: TYPE ID EQ type

type
: ID
| LBRACE type_fields RBRACE
| ARRAY OF ID

variable_declare
: VAR ID ASSIGN exp             
| VAR ID COLON ID ASSIGN exp    

function_declare
: FUNCTION ID LPAREN type_fields RPAREN EQ exp          
| FUNCTION ID LPAREN type_fields RPAREN COLON ID EQ exp 

/****************************************************************************
 * Fields
 ****************************************************************************/

type_fields
: /* epsilon */
| ID COLON ID
| ID COLON ID COMMA type_fields

sequence_fields
: exp
| exp SEMICOLON sequence_fields

argument_fields
: exp
| exp COMMA argument_fields

record_fields
: ID EQ exp
| ID EQ exp COMMA record_fields
