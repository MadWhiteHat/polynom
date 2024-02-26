%{
  #include <stdio.h>
  #include <stdint.h>
  #include <stdlib.h>
  #include <math.h>
%}

%code requires {
  #include "polynom.h"
  #include "variable_list.h"
}

%union {
  int64_t num;
  char letter;
  variable_t* variable;
  polynomial_t* polynomial;
}

%token<letter> LETTER
%token<letter> EXIT
%token<letter> PRINT
%token<letter> PRINT_VARS
%token<variable> VARIABLE
%token<num> NUMBER
%token<num> EOL

%type<num> line
%type<num> power
%type<polynomial> polynomial
%type<variable> variable

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
    EOL                                 { puts("Input string in empty"); }
    | PRINT_VARS EOL                    { print_variables_list(var_list); }
    | PRINT VARIABLE EOL                { print_variable($2); }
    | PRINT polynomial EOL              {
                                          print_polynomial($2);
                                          deallocate_polynomial($2);
                                        }
    | variable EOL                      {  }
    | EXIT EOL                          {
                                          remove_all_variables_list(&var_list);
                                          YYACCEPT;
                                        }

polynomial: 
    '(' polynomial ')'                  { $$ = $2; } 

    | polynomial '^' power              { $$ = pow_polynomial($1, $3); }

    | '-' polynomial %prec UNARY_MINUS  { $$ = neg_polynomial($2); }

    | polynomial '*' polynomial         {
                                          is_valid_operation($1, $3);
                                          $$ = mul_polynomials($1, $3);
                                        }
    | polynomial '+' polynomial         {
                                          is_valid_operation($1, $3);
                                          $$ = sum_polynomials($1, $3, '+');
                                        }
    | polynomial '-' polynomial         {
                                          is_valid_operation($1, $3);
                                          $$ = sum_polynomials($1, $3, '-');
                                        }
    | LETTER                            { $$ = create_polynomial(1, $1, 1); }
    | NUMBER                            { $$ = create_polynomial($1, 0, 0); }
    | NUMBER LETTER                     { $$ = create_polynomial($1, $2, 1); }
    | LETTER '^' power                  { $$ = create_polynomial(1, $1, $3); }
    | NUMBER LETTER '^' power           { $$ = create_polynomial($1, $2, $4); }

power: 
    NUMBER                              { $$ = $1; }
    | '(' power ')'                     { $$ = $2; }
    | '-' power %prec UNARY_MINUS       { $$ = -$2; }
    | power '^' power                   { $$ = (int64_t)pow($1, $3); }
    | power '*' power                   { $$ = $1 * $3; }
    | power '/' power                   { $$ = $1 / $3; }
    | power '%' power                   { $$ = $1 % $3; }
    | power '+' power                   { $$ = $1 + $3; }
    | power '-' power                   { $$ = $1 - $3; }

variable:
    VARIABLE '=' polynomial             {
                                          add_variable_list(&var_list, $1, $3);
                                        }
    | polynomial                        {
                                          add_variable_list(&var_list, NULL, $1);
                                          print_variable($$);
                                        }

%%
