all: clean
	@flex --lex-compat --posix polynom.l
	@bison -o polynom.yacc.c -d polynom.y -Wcounterexamples -Wconflicts-rr
	@gcc polynom.lex.c polynom.yacc.c main.c polynom.c variable_list.c \
		-o polynom -g -lm

clean:
	@rm -f polynom.lex* polynom.yacc* polynom

mem-run: all
	@valgrind --leak-check=full --show-leak-kinds=all ./polynom test.txt
run: all
	@./polynom test.txt

