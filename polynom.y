%{
  #include <stdio.h>
  #include <stdint.h>
  #include <stdlib.h>
  #include <math.h>
%}

%code requires {
  #include "polynom.h"
}

%union {
  int64_t num;
  char letter;
  polynomial_t* polynomial;
}

%token<letter> LETTER
%token<letter> EXIT
%token<num> NUMBER
%token<num> EOL

%type<num> line
%type<polynomial> polynomial
%type<num> power

%left '+' '-'
%left '*' '/' '%'
%right UNARY_MINUS
%right '^'
%nonassoc '(' ')'

%%

input: %empty
     | input line
     ;

line:
    EOL                                   { puts("Input string in empty"); }
    | polynomial EOL                      { print_polynomial($1); }
    | EXIT EOL                            { YYACCEPT; }

polynomial: 
    '(' polynomial ')'                    { $$ = $2; } 

    | polynomial '^' power                { $$ = pow_polynomial($1, $3); }

    | '-' polynomial %prec UNARY_MINUS    { $$ = neg_polynomial($2); }

    | polynomial polynomial %prec '*'     {
                                            is_valid_operation($1, $2);
                                            $$ = mul_polynomials($1, $2);
                                          }
    | polynomial '*' polynomial           {
                                            is_valid_operation($1, $3);
                                            $$ = mul_polynomials($1, $3);
                                          }
    | polynomial '+' polynomial           {
                                            is_valid_operation($1, $3);
                                            $$ = sum_polynomials($1, $3, '+');
                                          }
    | polynomial '-' polynomial           {
                                            is_valid_operation($1, $3);
                                            $$ = sum_polynomials($1, $3, '-');
                                          }
    | LETTER                              { $$ = init_polynomial(1, $1, 1); }
    | NUMBER                              { $$ = init_polynomial($1, 0, 0); }

power: 
     NUMBER                               { $$ = $1; }
     | '(' power ')'                      { $$ = $2; }
     | '-' power %prec UNARY_MINUS        { $$ = -$2; }
     | power '^' power                    { $$ = (int64_t)pow($1, $3); }
     | power '*' power                    { $$ = $1 * $3; }
     | power '/' power                    { $$ = $1 / $3; }
     | power '%' power                    { $$ = $1 % $3; }
     | power '+' power                    { $$ = $1 + $3; }
     | power '-' power                    { $$ = $1 - $3; }
