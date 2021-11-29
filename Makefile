test: test.o y.tab.o lex.yy.o ast.o show.o symbol.o util.o
	cc -g test.o y.tab.o lex.yy.o ast.o show.o symbol.o util.o

test.o: test.c ast.h util.h 
	cc -g -c test.c

y.tab.o: y.tab.c ast.h symbol.h util.h
	cc -g -c y.tab.c

y.tab.c: tiger.y
	yacc -dv tiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

lex.yy.o: lex.yy.c y.tab.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.flex
	lex tiger.flex

ast.o: ast.c ast.h
	cc -g -c ast.c

show.o: show.c show.h ast.h
	cc -g -c show.c

symbol.o: symbol.c util.h
	cc -g -c symbol.c

util.o: util.c util.h
	cc -g -c util.c
