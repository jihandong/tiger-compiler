typedef union
{
	int         pos;
	int         ival;
	const char *sval;
} YYSTYPE;

extern YYSTYPE yylval;

# define FIRST_TOKEN     257
# define TOKEN_ID        257
# define TOKEN_STRING    258
# define TOKEN_INT       259
# define TOKEN_COMMA     260
# define TOKEN_COLON     261
# define TOKEN_SEMICOLON 262
# define TOKEN_LPAREN    263
# define TOKEN_RPAREN    264
# define TOKEN_LBRACK    265
# define TOKEN_RBRACK    266
# define TOKEN_LBRACE    267
# define TOKEN_RBRACE    268
# define TOKEN_DOT       269
# define TOKEN_PLUS      270
# define TOKEN_MINUS     271
# define TOKEN_TIMES     272
# define TOKEN_DIVIDE    273
# define TOKEN_EQ        274
# define TOKEN_NEQ       275
# define TOKEN_LT        276
# define TOKEN_LE        277
# define TOKEN_GT        278
# define TOKEN_GE        279
# define TOKEN_AND       280
# define TOKEN_OR        281
# define TOKEN_ASSIGN    282
# define TOKEN_ARRAY     283
# define TOKEN_IF        284
# define TOKEN_THEN      285
# define TOKEN_ELSE      286
# define TOKEN_WHILE     287
# define TOKEN_FOR       288
# define TOKEN_TO        289
# define TOKEN_DO        290
# define TOKEN_LET       291
# define TOKEN_IN        292
# define TOKEN_END       293
# define TOKEN_OF        294
# define TOKEN_BREAK     295
# define TOKEN_NIL       296
# define TOKEN_FUNCTION  297
# define TOKEN_VAR       298
# define TOKEN_TYPE      299
# define LAST_TOKEN      299
