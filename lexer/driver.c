#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

#define TO_STR(x) #x

YYSTYPE yylval;
int yylex(void);
extern FILE *yyin;

static const char *token_names[] =
{
    TO_STR(TOKEN_ID),
    TO_STR(TOKEN_STRING),
    TO_STR(TOKEN_INT),
    TO_STR(TOKEN_COMMA),
    TO_STR(TOKEN_COLON),
    TO_STR(TOKEN_SEMICOLON),
    TO_STR(TOKEN_LPAREN),
    TO_STR(TOKEN_RPAREN),
    TO_STR(TOKEN_LBRACK),
    TO_STR(TOKEN_RBRACK),
    TO_STR(TOKEN_LBRACE),
    TO_STR(TOKEN_RBRACE),
    TO_STR(TOKEN_DOT),
    TO_STR(TOKEN_PLUS),
    TO_STR(TOKEN_MINUS),
    TO_STR(TOKEN_TIMES),
    TO_STR(TOKEN_DIVIDE),
    TO_STR(TOKEN_EQ),
    TO_STR(TOKEN_NEQ),
    TO_STR(TOKEN_LT),
    TO_STR(TOKEN_LE),
    TO_STR(TOKEN_GT),
    TO_STR(TOKEN_GE),
    TO_STR(TOKEN_AND),
    TO_STR(TOKEN_OR),
    TO_STR(TOKEN_ASSIGN),
    TO_STR(TOKEN_ARRAY),
    TO_STR(TOKEN_IF),
    TO_STR(TOKEN_THEN),
    TO_STR(TOKEN_ELSE),
    TO_STR(TOKEN_WHILE),
    TO_STR(TOKEN_FOR),
    TO_STR(TOKEN_TO),
    TO_STR(TOKEN_DO),
    TO_STR(TOKEN_LET),
    TO_STR(TOKEN_IN),
    TO_STR(TOKEN_END),
    TO_STR(TOKEN_OF),
    TO_STR(TOKEN_BREAK),
    TO_STR(TOKEN_NIL),
    TO_STR(TOKEN_FUNCTION),
    TO_STR(TOKEN_VAR),
    TO_STR(TOKEN_TYPE),
};

static inline const char *get_token_name(int token)
{
    if (token >= FIRST_TOKEN && token <= LAST_TOKEN)
        return token_names[token - FIRST_TOKEN];
    else
        return NULL;
}

int main(int argc, char **argv)
{
    int token;

    if (argc != 2) {
        fprintf(stderr, "usage: a.out filename\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        exit(1);
    }

    while(1) {
        token = yylex();
        if (token == 0)
            break;

        switch(token) {
            case TOKEN_ID:
            case TOKEN_STRING:
                printf("%s %s\n", get_token_name(token), yylval.sval);
                break;
            case TOKEN_INT:
                printf("%s %d\n", get_token_name(token), yylval.ival);
                break;
            default:
                printf("%s\n", get_token_name(token));
        }
    }

    fclose(yyin);
    return 0;
}
