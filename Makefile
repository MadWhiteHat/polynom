all: clean
	@echo 'Creating lex...'
	@flex --lex-compat --posix polynom.l
	@echo 'Done!'
	@echo 'Creating bison...'
	@bison -v -d polynom.y -Wcounterexamples -Wconflicts-rr -Wconflicts-sr
	@echo 'Done!'
	@echo 'Creating parser...'
	@gcc polynom.lex.c polynom.yacc.c main.c polynom.c variable_list.c \
		-o polynom -g -lm
	@echo 'Done!'

clean:
	@echo 'Cleaning up...'
	@rm -f polynom.lex* polynom.yacc* polynom
	@echo 'Done!'

mem-run: all
	@valgrind --leak-check=full --show-leak-kinds=all ./polynom test.txt
