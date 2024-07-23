%{

#include <stdlib.h>
#include <stdint.h>

#include "utility.h"
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
                            DEBUG_PRINT("Return number %ld\n", yylval.num);
                            return NUMBER;
                          }
{VAR}                     {
                            yylval.letter = *yytext;
                            DEBUG_PRINT("Return letter %c\n", yylval.letter);
                            return LETTER;
                          }
\$                        {
                            DEBUG_PRINT("Return symbol '$'");
                            BEGIN(VAR_DEF);
                            return *yytext;
                          }
\n                        { return EOL; }
<<EOF>>                   { return EXIT; }
"exit"                    { return EXIT; }
"echo"                    { return PRINT; } 
"env"                     { return PRINT_VARS;  }

<VAR_DEF>{ID}             {
                            DEBUG_PRINT("Return variable name:");
                            yylval.variable_name_info.buffer = yytext;
                            yylval.variable_name_info.length = yyleng;
                            BEGIN(INITIAL);
                            return VAR_NAME;
                          }
<VAR_DEF>.                { return INVALID_VARIABLE_NAME; }
{VALID_SYMS}              {
                            DEBUG_PRINT("Return symbol '%c'\n", *yytext);
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
