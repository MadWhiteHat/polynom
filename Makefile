all:
	@${MAKE} clean
	@flex -o polynom.lex.c polynom.l
	@bison -o polynom.yacc.c -d polynom.y -Wcounterexamples -Wconflicts-sr
	@gcc polynom.lex.c polynom.yacc.c main.c -o polynom -g -lm

clean:
	@rm -f polynom.lex* polynom.yacc* polynom
