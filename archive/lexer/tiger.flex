%{

#include <stdio.h>
#include "tokens.h"

static int comment_layer;

%}

%Start COMMENT
%%

<INITIAL>"/*"           { comment_layer++; BEGIN COMMENT; }
<INITIAL>","            { return TOKEN_COMMA; }
<INITIAL>":"            { return TOKEN_COLON; }
<INITIAL>";"            { return TOKEN_SEMICOLON; }
<INITIAL>"("            { return TOKEN_LPAREN; }
<INITIAL>")"            { return TOKEN_RPAREN; }
<INITIAL>"["            { return TOKEN_LBRACK; }
<INITIAL>"]"            { return TOKEN_RBRACK; }
<INITIAL>"{"            { return TOKEN_LBRACE; }
<INITIAL>"}"            { return TOKEN_RBRACE; }
<INITIAL>"."            { return TOKEN_DOT; }
<INITIAL>"+"            { return TOKEN_PLUS; }
<INITIAL>"-"            { return TOKEN_MINUS; }
<INITIAL>"*"            { return TOKEN_TIMES; }
<INITIAL>"/"            { return TOKEN_DIVIDE; }
<INITIAL>"="            { return TOKEN_EQ; }
<INITIAL>"<>"           { return TOKEN_NEQ; }
<INITIAL>"<"            { return TOKEN_LT; }
<INITIAL>"<="           { return TOKEN_LE; }
<INITIAL>">"            { return TOKEN_GT; }
<INITIAL>">="           { return TOKEN_GE; }
<INITIAL>"&"            { return TOKEN_AND; }
<INITIAL>"|"            { return TOKEN_OR; }
<INITIAL>":="           { return TOKEN_ASSIGN; }
<INITIAL>"array"        { return TOKEN_ARRAY; }
<INITIAL>"if"           { return TOKEN_IF; }
<INITIAL>"then"         { return TOKEN_THEN; }
<INITIAL>"else"         { return TOKEN_ELSE; }
<INITIAL>"while"        { return TOKEN_WHILE; }
<INITIAL>"for"          { return TOKEN_FOR; }
<INITIAL>"to"           { return TOKEN_TO; }
<INITIAL>"do"           { return TOKEN_DO; }
<INITIAL>"let"          { return TOKEN_LET; }
<INITIAL>"in"           { return TOKEN_IN; }
<INITIAL>"end"          { return TOKEN_END; }
<INITIAL>"of"           { return TOKEN_OF; }
<INITIAL>"break"        { return TOKEN_BREAK; }
<INITIAL>"nil"          { return TOKEN_NIL; }
<INITIAL>"function"     { return TOKEN_FUNCTION; }
<INITIAL>"var"          { return TOKEN_VAR; }
<INITIAL>"type"         { return TOKEN_TYPE; }
<INITIAL>[ \t\r]        { continue; }
<INITIAL>\n             { continue; }
<INITIAL>[0-9]+         { yylval.ival = atoi(yytext); return TOKEN_INT; }
<INITIAL>[a-zA-Z]\w*    { yylval.sval = strdup(yytext); return TOKEN_ID; }
<INITIAL>\"[^\"]*\"     { yylval.sval = strdup(yytext); return TOKEN_STRING; }
<INITIAL>.              { fprintf(stderr, "Error token: \"%s\"\n", yytext); }

<COMMENT>"/*"           { comment_layer++; continue; }
<COMMENT>"*/"           {
                            comment_layer--;
                            if (comment_layer > 0)
                                continue;
                            else if (comment_layer < 0)
                                fprintf(stderr, "Error comment layer\n");
                            else
                                BEGIN INITIAL;
                        }
<COMMENT>.              { continue; }

%%

int yywrap(void)
{
    return 1;
}
