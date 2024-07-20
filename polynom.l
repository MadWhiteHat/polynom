%{

#include <stdlib.h>
#include <stdint.h>

#include "errors.h"
#include "variable.h"
#include "polynom.yacc.h"
%}

%array

%option noyywrap
%option outfile="polynom.lex.c" header-file="polynom.lex.h"

DIGIT [0-9]
VAR [a-z]
ID [_a-zA-Z0-9]+
SKIPPABLE [ \t\r]
VALID_SYMS [+\-*/%^()$=]

%x VAR_DEF

%%

{SKIPPABLE}+              ;
{DIGIT}+                  {
                            yylval.num = atoi(yytext);
                            printf("Return number %ld\n", yylval.num);
                            return NUMBER;
                          }
{VAR}                     {
                            yylval.letter = *yytext;
                            printf("Return letter %c\n", yylval.letter);
                            return LETTER;
                          }
\$                        {
                            puts("Return symbol '$'");
                            BEGIN(VAR_DEF);
                            return *yytext;
                          }
\n                        { return EOL; }
<<EOF>>                   { return EXIT; }
"exit"                    { return EXIT; }
"echo"                    { return PRINT; } 
"env"                     { return PRINT_VARS;  }

<VAR_DEF>{ID}             {
                            yylval.variable_name = create_variable_name(
                              yytext, yyleng
                            );
                            puts("Return variable name:");
                            print_variable_name(yylval.variable_name);
                            BEGIN(INITIAL);
                            return VAR_NAME;
                          }
<VAR_DEF>.                { return INVALID_VARIABLE_NAME; }
{VALID_SYMS}              {
                            printf("Return symbol '%c'\n", *yytext);
                            return *yytext;
                          }
.                         {
                            print_error(
                              LEXICAL, "invalid token \'%c\'", *yytext
                            );
                            delete_tree(root);
                            exit(-1);
                          }
%%
