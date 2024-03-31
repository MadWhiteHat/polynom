%header "polynom.yacc.h"
%output "polynom.yacc.c"
%code top {
  #include <stdio.h>
  #include <stdint.h>
  #include <math.h>
}

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
%token<num> PRINT
%token<num> PRINT_VARS
%token<num> NUMBER
%token<num> EOL
%token<variable> VARIABLE

%type<num> line
%type<num> power
%type<polynomial> monomial
%type<polynomial> polynomial1
%type<polynomial> polynomial2
%type<polynomial> polynomial3
%type<polynomial> polynomial4
%type<variable> variable

%right '='
%left '+' '-'
%left '*' '/' '%'
%right UMINUS
%right '^'
%nonassoc '(' ')'

%%

input: %empty
    | input line
    ;

line:
    EOL                               { puts("Input string in empty"); }
    | PRINT_VARS EOL                  { print_variables_list(var_list); }
    | PRINT VARIABLE EOL              { print_variable($2); }
    | PRINT polynomial1 EOL           {
                                        print_polynomial($2);
                                        deallocate_polynomial($2);
                                      }
    | variable EOL                    { }
    | YYEOF                           {
                                        remove_all_variables_list(&var_list);
                                        YYACCEPT;
                                      }

polynomial4:
    polynomial4 '^' power             { $$ = pow_polynomial($1, $3); }
    | '(' polynomial1 ')'             { $$ = $2; }
    | monomial

polynomial3:
    '-' polynomial4 %prec UMINUS      { $$ = neg_polynomial($2); }
    | polynomial4                     { }


polynomial2:
    polynomial2 '*' polynomial3       {
                                        is_valid_operation($1, $3);
                                        $$ = mul_polynomials($1, $3);
                                      }
    | polynomial2 polynomial4         {
                                        is_valid_operation($1, $2);
                                        $$ = mul_polynomials($1, $2);
                                      }
    | polynomial3                     { }

polynomial1:
      polynomial1 '+' polynomial2     {
                                        is_valid_operation($1, $3);
                                        $$ = sum_polynomials($1, $3, '+');
                                      }
    | polynomial1 '-' polynomial2     {
                                        is_valid_operation($1, $3);
                                        $$ = sum_polynomials($1, $3, '-');
                                      }
    | polynomial2                     { }

monomial:
    LETTER                            { $$ = create_polynomial(1, $1, 1); }
    | NUMBER                          { $$ = create_polynomial($1, 0, 0); }

power:
    NUMBER                            { $$ = $1; }

    | '-' power %prec UMINUS          { $$ = -$2; }

    | '(' power ')'                   { $$ = $2; }

    | power '^' power                 { $$ = (int64_t)pow($1, $3); }

    | power '*' power                 { $$ = $1 * $3; }
    | power '/' power                 { $$ = $1 / $3; }
    | power '%' power                 { $$ = $1 % $3; }

    | power '+' power                 { $$ = $1 + $3; }
    | power '-' power                 { $$ = $1 - $3; }

variable:
    VARIABLE '=' polynomial1          {
                                        add_variable_list(&var_list, $1, $3);
                                      }
    | polynomial1                     {
                                        add_variable_list(&var_list, NULL, $1);
                                        print_polynomial($1);
                                      }

%%
