#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "polynom.h"
#include "polynom.lex.h"
#include "polynom.yacc.h"

void yyerror(const char* __msg) {
  printf("ERROR: %s\n", __msg);
}

extern FILE* yyin;

int32_t main(int32_t argc, const char** argv) {
  printf("Welcome to polynomial calculator!\n");
  if (argc == 2) {
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
      puts("Cannot open specified file");
      return 1;
    }
    YY_BUFFER_STATE input_buffer = yy_create_buffer(input, YY_BUF_SIZE);
    yy_switch_to_buffer(input_buffer);
    yyparse();
    yy_delete_buffer(input_buffer);
    fclose(input);
  } else {
    yyin = stdin;
    yyparse();
  }
  yylex_destroy();

  return 0;
}
