test: test.o y.tab.o lex.yy.o ast.o env.o print.o semant.o symbol.o table.o type.o util.o
	cc -g *.o

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

env.o: env.c
	cc -g -c env.c

print.o: print.c
	cc -g -c print.c -Wincompatible-pointer-types

symbol.o: symbol.c
	cc -g -c symbol.c -Wincompatible-pointer-types -Wint-conversion

table.o: table.c
	cc -g -c table.c -Wincompatible-pointer-types -Wint-conversion

type.o: type.c
	cc -g -c type.c

util.o: util.c
	cc -g -c util.c

# clean
clean:
	rm -rf a.out *.o lex.yy.c y.tab.c y.tab.h y.output
