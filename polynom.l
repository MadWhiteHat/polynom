%{

#include <stdlib.h>
#include <stdint.h>

#include "variable_list.h"
#include "polynom.yacc.h"
%}

%array

%option noyywrap
%option outfile="polynom.lex.c" header-file="polynom.lex.h"

DIGIT [0-9]
VAR [a-z]
ID [_a-zA-Z][_a-zA-Z0-9]*
SKIPPABLE [ \t\r]

%%

{SKIPPABLE}+              ;
{DIGIT}+                  { yylval.num = atoi(yytext); return NUMBER; }
{VAR}                     { yylval.letter = *yytext; return LETTER; }
\${ID}                    {
                            yylval.variable = create_variable(
                              yytext + 1,
                              yyleng - 1
                            );
                            return VARIABLE;
                          }
\n                        { return EOL; }
<<EOF>>                   { return YYEOF; }
"exit"                    { return YYEOF; }
"print"                   { return PRINT; } 
"print vars"              { return PRINT_VARS;  }
[^\*\/\+\-\r\n\(= ]\(     {
                            char tmp = yytext[0];
                            unput('(');
                            unput('*');
                            unput(tmp);
                          }
\)[^\*\/\+\-\^\r\n\)]     {
                            yyless(1);
                            unput('*');
                            unput(')');
                          }
.                         { return *yytext; }

%%
