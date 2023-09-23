%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  #include "variable_list.h"
  #include "polynom.yacc.h"
%}

%option noyywrap

%%

[ \t\r]             ; // whitespace
[0-9]+              { yylval.num = atoi(yytext); return NUMBER; }
[a-z]               { yylval.letter = *yytext; return LETTER; }
\$[_a-zA-Z0-9]+     {
                      yylval.variable = create_variable(yytext + 1, yyleng - 1);
                      return VARIABLE;
                    }
\n                  { return EOL; }
"exit"              { return EXIT; }
"print"             { return PRINT; }
")("                {
                      unput('(');
                      unput('*');
                      unput(')');
                    }
[^\*\/\+\-\r\n]\(   {
                      char tmp = yytext[0];
                      unput('(');
                      unput('*');
                      unput(tmp);
                    }
\)[^\*\/\+\-\^\r\n] {
                      unput(yytext[1]);
                      unput('*');
                      unput(')');
                    }
.                   { return *yytext; }

%%
