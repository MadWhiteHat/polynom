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
	@gcc polynom.lex.c polynom.yacc.c \
		polynom.c variable.c variable_tree.c errors.c \
		main.c \
		 -Wall -Wextra -pedantic -o polynom -g -lm
	@echo 'Done!'

clean:
	@echo 'Cleaning up...'
	@rm -f polynom.lex* polynom.yacc* polynom
	@rm -rf output sorted
	@echo 'Done!'

all: clean parser
