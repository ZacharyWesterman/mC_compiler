GRAMMAR = parser.y

CFLAGS = -I. -funsigned-char -g -DYYDEBUG
YFLAGS = -v -d

mcc: y.tab.o lex.yy.o tree.o strtab.o symtab.o semantics.o func_signature.o driver.o
	gcc $(CFLAGS) -o mcc y.tab.o lex.yy.o tree.o strtab.o symtab.o semantics.o func_signature.o driver.o -lfl

driver.o: driver.c
	gcc $(CFLAGS) -c driver.c

y.tab.o: y.tab.c y.tab.h 
	gcc $(CFLAGS) -c y.tab.c 

y.tab.c: $(GRAMMAR)
	yacc $(YFLAGS) $(GRAMMAR)

lex.yy.o: lex.yy.c y.tab.h 
	gcc $(CFLAGS) -c lex.yy.c

lex.yy.c: scanner.l
	flex scanner.l

tree.o: tree.c tree.h
	gcc $(CFLAGS) -c tree.c

strtab.o: strtab.c strtab.h
	gcc $(CFLAGS) -c strtab.c

symtab.o: symtab.c symtab.h
	gcc $(CFLAGS) -c symtab.c

semantics.o: semantics.c semantics.h
	gcc $(CFLAGS) -c semantics.c

func_signature.o: func_signature.c func_signature.h
	gcc $(CFLAGS) -c func_signature.c

clean:
	rm -f y.tab.* y.output lex.yy.* *.o *~ mcc     



