parsetest: tester.o y.tab.o lex.yy.o
	cc -g -o parsetest tester.o y.tab.o lex.yy.o

tester.o: tester.c
	cc -g -c tester.c

y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tiger.y
	yacc -dv tiger.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

lex.yy.o: lex.yy.c y.tab.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.flex
	flex tiger.flex

clean:
	rm -f parsetest tester.o lex.yy.o lex.yy.c y.tab.c y.tab.h y.tab.o y.output