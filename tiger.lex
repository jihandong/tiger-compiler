%{

#include <stdio.h>
#include "ast.h"
#include "y.tab.h"

#define RETURN(x) ({ printf("%10s %10s\n", #x, yytext); return x; })

static int comment_layer;

%}

%Start COMMENT

%%

<INITIAL>"/*"           { comment_layer++; BEGIN COMMENT; }
<INITIAL>","            { RETURN(COMMA); }
<INITIAL>":"            { RETURN(COLON); }
<INITIAL>";"            { RETURN(SEMICOLON); }
<INITIAL>"("            { RETURN(LPAREN); }
<INITIAL>")"            { RETURN(RPAREN); }
<INITIAL>"["            { RETURN(LBRACK); }
<INITIAL>"]"            { RETURN(RBRACK); }
<INITIAL>"{"            { RETURN(LBRACE); }
<INITIAL>"}"            { RETURN(RBRACE); }
<INITIAL>"."            { RETURN(DOT); }
<INITIAL>"+"            { RETURN(PLUS); }
<INITIAL>"-"            { RETURN(MINUS); }
<INITIAL>"*"            { RETURN(TIMES); }
<INITIAL>"/"            { RETURN(DIVIDE); }
<INITIAL>"="            { RETURN(EQ); }
<INITIAL>"<>"           { RETURN(NEQ); }
<INITIAL>"<"            { RETURN(LT); }
<INITIAL>"<="           { RETURN(LE); }
<INITIAL>">"            { RETURN(GT); }
<INITIAL>">="           { RETURN(GE); }
<INITIAL>"&"            { RETURN(AND); }
<INITIAL>"|"            { RETURN(OR); }
<INITIAL>":="           { RETURN(ASSIGN); }
<INITIAL>"array"        { RETURN(ARRAY); }
<INITIAL>"if"           { RETURN(IF); }
<INITIAL>"then"         { RETURN(THEN); }
<INITIAL>"else"         { RETURN(ELSE); }
<INITIAL>"while"        { RETURN(WHILE); }
<INITIAL>"for"          { RETURN(FOR); }
<INITIAL>"to"           { RETURN(TO); }
<INITIAL>"do"           { RETURN(DO); }
<INITIAL>"let"          { RETURN(LET); }
<INITIAL>"in"           { RETURN(IN); }
<INITIAL>"end"          { RETURN(END); }
<INITIAL>"of"           { RETURN(OF); }
<INITIAL>"break"        { RETURN(BREAK); }
<INITIAL>"nil"          { RETURN(NIL); }
<INITIAL>"function"     { RETURN(FUNCTION); }
<INITIAL>"var"          { RETURN(VAR); }
<INITIAL>"type"         { RETURN(TYPE); }
<INITIAL>[ \t\r]        { continue; }
<INITIAL>\n             { continue; }
<INITIAL>[0-9]+         { yylval.ival = atoi(yytext); RETURN(INT); }
<INITIAL>\"[^\"]*\"     { yylval.sval = strdup(yytext); RETURN(STRING); }
<INITIAL>[a-zA-Z][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); RETURN(ID); }
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
