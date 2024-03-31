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
%type<polynomial> mono
%type<polynomial> poly_add_prio
%type<polynomial> poly_mul_prio
%type<polynomial> poly_neg_prio
%type<polynomial> poly_pow_prio
%type<variable> variable

%right '='
%left '+' '-'
%left '*' '/' '%'
%right UMINUS
%right '^'
%nonassoc '(' ')'

%%

input: 
  %empty
  | input line
  ;

line:
  EOL                               { puts("Input string in empty"); }
  | PRINT_VARS EOL                  { print_variables_list(var_list); }
  | PRINT VARIABLE EOL              { print_variable($2); }
  | PRINT poly_add_prio EOL           {
                                      print_polynomial($2);
                                      deallocate_polynomial($2);
                                    }
  | variable EOL                    { }
  | YYEOF                           {
                                      remove_all_variables_list(&var_list);
                                      YYACCEPT;
                                    }

poly_pow_prio:
  poly_pow_prio '^' power           { $$ = pow_polynomial($1, $3); }
  | '(' poly_add_prio ')'           { $$ = $2; }
  | mono

poly_neg_prio:
  '-' poly_pow_prio %prec UMINUS    { $$ = neg_polynomial($2); }
  | poly_pow_prio                   { }


poly_mul_prio:
  poly_mul_prio '*' poly_neg_prio   {
                                      is_valid_operation($1, $3);
                                      $$ = mul_polynomials($1, $3);
                                    }
  | poly_mul_prio poly_pow_prio     {
                                      is_valid_operation($1, $2);
                                      $$ = mul_polynomials($1, $2);
                                    }
  | poly_neg_prio                   { }

poly_add_prio:
    poly_add_prio '+' poly_mul_prio {
                                      is_valid_operation($1, $3);
                                      $$ = sum_polynomials($1, $3, '+');
                                    }
  | poly_add_prio '-' poly_mul_prio {
                                      is_valid_operation($1, $3);
                                      $$ = sum_polynomials($1, $3, '-');
                                    }
  | poly_mul_prio                   { }

mono:
  LETTER                            { $$ = create_polynomial(1, $1, 1); }
  | NUMBER                          { $$ = create_polynomial($1, 0, 0); }

power:
  NUMBER                            { $$ = $1; }

  | '(' power ')'                   { $$ = $2; }

  | power '^' power                 { $$ = (int64_t)pow($1, $3); }

  | '-' power %prec UMINUS          { $$ = -$2; }

  | power '*' power                 { $$ = $1 * $3; }
  | power '/' power                 { $$ = $1 / $3; }
  | power '%' power                 { $$ = $1 % $3; }

  | power '+' power                 { $$ = $1 + $3; }
  | power '-' power                 { $$ = $1 - $3; }

variable:
  VARIABLE '=' poly_add_prio        {
                                      add_variable_list(&var_list, $1, $3);
                                    }
  | poly_add_prio                   {
                                      add_variable_list(&var_list, NULL, $1);
                                      print_polynomial($1);
                                    }

%%
