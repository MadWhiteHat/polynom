lex:
	@echo 'Creating lex...'
	@flex --lex-compat --posix polynom.l
	@echo 'Done!'

yacc: lex
	@echo 'Creating yacc...'
	@bison -v -d polynom.y -Wcounterexamples -Wconflicts-rr -Wconflicts-sr
	@echo 'Done!'

parser: yacc
	@echo 'Creating parser...'
	@gcc polynom.lex.c polynom.yacc.c polynom.c variable.c variable_tree.c main.c \
		-Wall -Wextra -pedantic -o polynom -g -lm
	@echo 'Done!'

test: variable.c test_main.c polynom.c variable_tree.c
	@gcc variable.c test_main.c polynom.c variable_tree.c -o test -g

clean:
	@echo 'Cleaning up...'
	@rm -f polynom.lex* polynom.yacc* polynom
	@rm -rf output
	@echo 'Done!'

all: clean parser

mem-run: all
	@valgrind --leak-check=full --show-leak-kinds=all ./polynom test.txt
