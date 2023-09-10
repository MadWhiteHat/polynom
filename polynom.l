%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "polynom.yacc.h"
%}

%option noyywrap

%%

[ \t\r]       ; // whitespace
[0-9]+        { yylval.num = atoi(yytext); return NUMBER; }
[a-z]         { yylval.letter = *yytext; return LETTER;}
[\n]          { return EOL; }
"exit"        { return EXIT; }
")("          { unput('('); unput('*'); unput(')'); }
.             { printf("%s\n", yytext); return *yytext; }

%%
