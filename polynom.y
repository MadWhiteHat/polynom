%header "polynom.yacc.h"
%output "polynom.yacc.c"
%code top {
  #include <stdio.h>
  #include <stdint.h>
  #include <stdlib.h>
  #include <math.h>
}

%code requires {
  #include "errors.h"
  #include "polynom.h"
  #include "variable.h"
  #include "variable_tree.h"
}

%union {
  int64_t num;
  char letter;
  variable_name_t* variable_name;
  variable_t* variable;
  polynomial_t* polynomial;
}

%token<letter> LETTER
%token<num> PRINT
%token<num> PRINT_VARS
%token<num> NUMBER
%token<num> EOL
%token<num> EXIT
%token<num> INVALID_VARIABLE_NAME
%token<variable_name> VAR_NAME

%type<num> line
%type<num> pow_add
%type<num> pow_mul
%type<num> pow_neg
%type<num> pow_pow
%type<polynomial> mono
%type<polynomial> poly
%type<polynomial> poly_add
%type<polynomial> poly_mul
%type<polynomial> poly_neg
%type<polynomial> poly_pow
%type<variable> var
%type<variable> var_eq
%type<variable> var_add
%type<variable> var_mul
%type<variable> var_neg
%type<variable> var_pow

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
  EOL                         { ++line; }
  | error EOL                 {
                                print_error(SYNTAX, "unknown syntax");
                              }
  | PRINT_VARS EOL            { print_tree(root); ++line; }
  | PRINT '$' VAR_NAME EOL    { print_variable_by_name($3); ++line; }
  | PRINT poly_add EOL        {
                                print_polynomial($2);
                                delete_polynomial($2);
                                ++line;
                              }
  | var_eq EOL                {
                                is_valid_variable($1);
                                is_initialized_variable($1);
                                try_delete_variable($1);
                                ++line;
                              }
  | EXIT                      {
                                delete_tree(root);
                                YYACCEPT;
                              }

poly_add:
    poly_add '+' poly_mul     {
                                is_valid_polynomial_operation($1, $3);
                                $$ = sum_polynomials($1, $3, '+');
                                delete_polynomial($1);
                                delete_polynomial($3);
                              }
  | poly_add '-' poly_mul     {
                                is_valid_polynomial_operation($1, $3);
                                $$ = sum_polynomials($1, $3, '-');
                                delete_polynomial($1);
                                delete_polynomial($3);
                              }
  | poly_add '+' '+' poly_mul {
                                print_error(SYNTAX,
                                  "Ill formed addition expression"
                                );
                                exit(-1);
                              }
  | poly_mul                  { $$ = $1; }

poly_mul:
  poly_mul '*' poly_neg       {
                                is_valid_polynomial_operation($1, $3);
                                $$ = mul_polynomials($1, $3);
                                delete_polynomial($3);
                                delete_polynomial($1);
                              }
  | poly_mul poly_pow         {
                                is_valid_polynomial_operation($1, $2);
                                $$ = mul_polynomials($1, $2);
                                delete_polynomial($2);
                                delete_polynomial($1);
                              }
  | poly_mul '/' poly_neg      {
                                //Invalid operation
                                print_error(SYNTAX,
                                  "polynomial division in not supported"
                                );
                                exit(-1);
                              }
  | poly_neg                  { $$ = $1; }

poly_neg:
  '-' poly_neg %prec UMINUS   {
                                $$ = neg_polynomial($2);
                                delete_polynomial($2);
                              }
  | poly_pow                  { $$ = $1; }

poly_pow:
  poly_pow '^' pow_pow        {
                                $$ = pow_polynomial($1, $3);
                                delete_polynomial($1);
                              }
  | poly                      { $$ = $1; }

poly:
  '(' poly_add ')'            { $$ = $2; }
  | mono                      { $$ = $1; }

mono:
  LETTER                      { $$ = create_polynomial(1, $1, 1); }
  | NUMBER                    { $$ = create_polynomial($1, 0, 0); }

pow_add:
  pow_add '+' pow_mul         { $$ = $1 + $3; }
  | pow_add '-' pow_mul       { $$ = $1 - $3; }
  | pow_mul                   { }

pow_mul:
  pow_mul '*' pow_neg         { $$ = $1 * $3; }
  | pow_mul '/' pow_neg       { $$ = $1 / $3; }
  | pow_mul '%' pow_neg       { $$ = $1 % $3; }
  | pow_neg                   {  }

pow_neg:
  '-' pow_neg %prec UMINUS    { $$ = -$2; }
  | pow_pow                   { }

pow_pow:
  pow_pow '^' pow_pow         { $$ = (int64_t)pow($1, $3); }
  | '(' pow_add ')'           { $$ = $2; }
  | NUMBER                    { $$ = $1; }

var_eq:
  var_add '=' var_eq          {
                                puts("Rule 1");
                                is_persistent_variable($1);
                                is_valid_variable($3);
                                is_initialized_variable($3);
                                $$ = create_variable(
                                  copy_variable_name($1->name),
                                  copy_polynomial($3->polynomial)
                                );
                                try_delete_variable($1);
                                try_delete_variable($3);
                                print_variable($$);
                                root = insert(root, $$);
                              }
  | var_add                   {
                                puts("Rule 2");
                                $$ = $1;
                                print_variable($$);
                              }

var_add:
  var_add '+' var_mul         {
                                puts("Rule 3");
                                is_valid_variable_operation($1, $3);
                                $$ = sum_variables($1, $3, '+');
                                try_delete_variable($1);
                                try_delete_variable($3);
                                print_variable($$);
                              }
  | var_add '-' var_mul       {
                                puts("Rule 4");
                                is_valid_variable_operation($1, $3);
                                $$ = sum_variables($1, $3, '-');
                                try_delete_variable($1);
                                try_delete_variable($3);
                                print_variable($$);
                              }
  | var_mul                   {
                                puts("Rule 5");
                                $$ = $1;
                                print_variable($$);
                              }

var_mul:
  var_mul '*' var_neg         {
                                puts("Rule 6");
                                is_valid_variable_operation($1, $3);
                                $$ = mul_variables($1, $3);
                                try_delete_variable($1);
                                try_delete_variable($3);
                                print_variable($$);
                              }
  | var_mul var_pow           {
                                puts("Rule 7");
                                is_valid_variable_operation($1, $2);
                                $$ = mul_variables($1, $2);
                                try_delete_variable($1);
                                try_delete_variable($2);
                                print_variable($$);
                              }
  | var_neg                   {
                                puts("Rule 8");
                                $$ = $1;
                                print_variable($$);
                              }

var_neg:
  '-' var_neg %prec UMINUS    {
                                puts("Rule 9");
                                is_valid_variable($2);
                                is_initialized_variable($2);
                                $$ = neg_variable($2);
                                try_delete_variable($2);
                                print_variable($$);
                              }
  | var_pow                   {
                                puts("Rule 10");
                                $$ = $1;
                                print_variable($$);
                              }

var_pow:
  var '^' var_pow             {
                                puts("Rule 11");
                                is_valid_variable_operation($1, $3);
                                $$ = pow_variable(
                                  $1,
                                  convert_polynomial_to_power($3->polynomial)
                                );
                                try_delete_variable($1);
                                try_delete_variable($3);
                                print_variable($$);
                              }
  | var                       {
                                puts("Rule 12");
                                $$ = $1;
                                print_variable($$);
                              }

var:
  '$' VAR_NAME                {
                                // Try to find variable in tree
                                // If not found create new variable with
                                // NULL polynomial
                                puts("Rule 13");
                                $$ = find_variable_by_name(root, $2);
                                if ($$ == NULL) {
                                  $$ = create_variable($2, NULL);
                                } else { delete_variable_name($2); }
                                print_variable($$);
                              }
  | '(' var_eq ')'            {
                                puts("Rule 14");
                                $$ = $2;
                                print_variable($$);
                              }
  | mono                      {
                                puts("Rule 15");
                                $$ = create_variable(NULL, $1);
                                print_variable($$);
                              }


%%
