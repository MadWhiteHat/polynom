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
  if (argc == 2) {
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
      puts("Cannot open specified file");
      return 1;
    }
    yyparse();
    fclose(yyin);
  } else {
    yyin = stdin;
    yyparse();
  }
  yylex_destroy();

  return 0;
}
