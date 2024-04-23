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
%token<num> EXIT
%token<variable> VARIABLE

%type<num> line
%type<num> pow_add
%type<num> pow_mul
%type<num> pow_neg
%type<num> pow_pow
%type<polynomial> mono
%type<polynomial> poly_add
%type<polynomial> poly_mul
%type<polynomial> poly_neg
%type<polynomial> poly_pow
%type<variable> variable

%right '='
%left '+' '-'
%left '*' '/' '%'
%right UMINUS
%right '^'
%nonassoc '(' ')'

%start input

%%

input:
  input line
  | line
  ;

line:
  EOL                               { puts("Input string in empty"); }
  | PRINT_VARS EOL                  { print_variables_list(var_list); }
  | PRINT VARIABLE EOL              { print_variable($2); }
  | PRINT poly_add EOL              {
                                      print_polynomial($2);
                                      deallocate_polynomial($2);
                                    }
  | variable EOL                    { }
  | EXIT                            {
                                      remove_all_variables_list(&var_list);
                                      YYACCEPT;
                                    }

poly_pow:
  poly_pow '^' pow_pow              {
                                      $$ = pow_polynomial($1, $3);
                                      deallocate_polynomial($1);
                                    }
  | '(' poly_add ')'                { $$ = $2; }
  | mono

poly_neg:
  '-' poly_neg %prec UMINUS         { $$ = neg_polynomial($2); }
  | poly_pow                        { }


poly_mul:
  poly_mul '*' poly_neg             {
                                      is_valid_operation($1, $3);
                                      $$ = mul_polynomials($1, $3);
                                      deallocate_polynomial($3);
                                      deallocate_polynomial($1);
                                    }
  | poly_mul poly_pow               {
                                      is_valid_operation($1, $2);
                                      $$ = mul_polynomials($1, $2);
                                      deallocate_polynomial($2);
                                      deallocate_polynomial($1);
                                    }
  | poly_neg                        { }

poly_add:
    poly_add '+' poly_mul           {
                                      is_valid_operation($1, $3);
                                      $$ = sum_polynomials($1, $3, '+');
                                    }
  | poly_add '-' poly_mul           {
                                      is_valid_operation($1, $3);
                                      $$ = sum_polynomials($1, $3, '-');
                                    }
  | poly_mul                        { }

mono:
  LETTER                            { $$ = create_polynomial(1, $1, 1); }
  | NUMBER                          { $$ = create_polynomial($1, 0, 0); }

pow_add:
  pow_add '+' pow_mul               { $$ = $1 + $3; }
  | pow_add '-' pow_mul             { $$ = $1 - $3; }
  | pow_mul                         { }

pow_mul:
  pow_mul '*' pow_neg               { $$ = $1 * $3; }
  | pow_mul '/' pow_neg             { $$ = $1 / $3; }
  | pow_mul '%' pow_neg             { $$ = $1 % $3; }
  | pow_neg                         {  }

pow_neg:
  '-' pow_neg %prec UMINUS          { $$ = -$2; }
  | pow_pow                         { }

pow_pow:
  pow_pow '^' pow_pow               { $$ = (int64_t)pow($1, $3); }
  | '(' pow_add ')'                 { $$ = $2; }
  | NUMBER                          { $$ = $1; }

variable:
  VARIABLE '=' poly_add             {
                                      add_variable_list(&var_list, $1, $3);
                                    }
  | poly_add                        {
                                      add_variable_list(&var_list, NULL, $1);
                                      print_polynomial($1);
                                    }

%%
