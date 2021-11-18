lextest: driver.o lex.yy.o
	cc -g -o lextest driver.o lex.yy.o

driver.o: driver.c tokens.h
	cc -g -c driver.c

lex.yy.o: lex.yy.c tokens.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.flex
	flex tiger.flex

clean:
	rm -f lextest driver.o lex.yy.o lex.yy.c