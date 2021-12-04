test: test.o y.tab.o lex.yy.o semant.o ast.o astpr.o symbol.o util.o
	cc -g test.o y.tab.o lex.yy.o ast.o astpr.o symbol.o util.o

test.o: test.c
	cc -g -c test.c

# lexer
lex.yy.o: lex.yy.c y.tab.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.lex
	lex tiger.lex

# parser
y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tiger.y
	yacc -dv tiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

# analyser
semant.o: semant.c
	cc -g -c semant.c

# common
ast.o: ast.c
	cc -g -c ast.c

astpr.o: astpr.c
	cc -g -c astpr.c -Wincompatible-pointer-types

symbol.o: symbol.c
	cc -g -c symbol.c -Wincompatible-pointer-types -Wint-conversion

type.o: type.c
	cc -g -c type.c

util.o: util.c
	cc -g -c util.c

# clean
clean:
	rm -rf a.out *.o lex.yy.c y.tab.c y.tab.h y.output
