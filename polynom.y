%header "polynom.yacc.h"
%output "polynom.yacc.c"
%code top {
  #include <stdio.h>
  #include <stdint.h>
  #include <stdlib.h>
  #include <math.h>
}

%code requires {
  #include "utility.h"
  #include "polynom.h"
  #include "variable.h"
  #include "variable_tree.h"
}

%union {
  int64_t num;
  char letter;
  variable_name_info_t variable_name_info;
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
%token<variable_name_info> VAR_NAME

%type<num> line
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
  EOL                         {
                                // Init

                                // Perform an action
                                ++line;
                                err = ERROR_INVALID_SYNTAX;

                                // Cleanup
                              }
  | error EOL                 {
                                // Init

                                // Perform an action
                                print_error(SYNTAX, "unknown syntax");
                                ++line;
                                err = ERROR_SUCCESS;

                                // Cleanup
                              }
  | PRINT_VARS EOL            {
                                // Init

                                // Perform an action
                                print_tree(root);
                                ++line;
                                err = ERROR_SUCCESS;

                                // Cleanup
                              }
  | PRINT '$' VAR_NAME EOL    {
                                // Init
                                variable_name_t* var_name = NULL;

                                // Perform an action
                                err = create_variable_name(
                                  &var_name, $3.buffer, $3.length
                                );

                                print_variable_name(var_name);
                                if (SUCCESS(err)) {
                                  print_variable_by_name(root, var_name);
                                }
                                ++line;
                                err = ERROR_SUCCESS;

                                // Cleanup
                                delete_variable_name(&var_name);
                              }
  | PRINT poly_add EOL        {
                                // Init

                                // Perform an action
                                print_polynomial($2);
                                ++line;
                                err = ERROR_SUCCESS;

                                // Cleanup
                                delete_polynomial(&$2);
                              }
  | var_eq EOL                {
                                // Init

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = is_valid_variable($1);
                                }

                                if (SUCCESS(err)) {
                                  err = is_initialized_variable($1);
                                }
                                ++line;
                                err = ERROR_SUCCESS;

                                // Cleanup
                                try_delete_variable(&$1);
                              }
  | EXIT                      {
                                // Init

                                // Perform an action
                                delete_tree(root);
                                YYACCEPT;

                                // Cleanup
                              }

poly_add:
    poly_add '+' poly_mul     {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = sum_polynomials(
                                    &res_polynomial, $1, $3, '+'
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$3);
                              }
  | poly_add '-' poly_mul     {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = sum_polynomials(
                                    &res_polynomial, $1, $3, '-'
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$3);
                              }
  | poly_add '+' '+' poly_mul {
                                // Init

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = ERROR_INVALID_SYNTAX;
                                  print_error(SYNTAX,
                                    "Ill formed addition expression"
                                  );
                                }

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$4);
                              }
  | poly_mul                  { $$ = $1; }

poly_mul:
  poly_mul '*' poly_neg       {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = mul_polynomials(
                                    &res_polynomial, $1, $3
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$3);
                              }
  | poly_mul poly_pow         {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = mul_polynomials(
                                    &res_polynomial, $1, $2
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$2);
                              }
  | poly_mul '/' poly_neg     {
                                // Init

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = ERROR_INVALID_SYNTAX;
                                  print_error(SYNTAX,
                                    "polynomial division in not supported"
                                  );
                                }

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$3);
                              }
  | poly_neg                  { $$ = $1; }

poly_neg:
  '-' poly_neg %prec UMINUS   {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = neg_polynomial(&res_polynomial, $2);
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                                delete_polynomial(&$2);
                              }
  | poly_pow                  { $$ = $1; }

poly_pow:
  poly '^' poly_pow           {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = pow_polynomial(&res_polynomial, $1, $3);
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                                delete_polynomial(&$1);
                                delete_polynomial(&$3);
                              }
  | poly                      { $$ = $1; }

poly:
  '(' poly_add ')'            { $$ = $2; }
  | mono                      { $$ = $1; }

mono:
  LETTER                      {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = create_polynomial(
                                    &res_polynomial, 1, $1, 1
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                              }
  | NUMBER                    {
                                // Init
                                polynomial_t* res_polynomial = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = create_polynomial(
                                    &res_polynomial, $1, 0, 0
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_polynomial : NULL;

                                // Cleanup
                              }

var_eq:
  var_add '=' var_eq          {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = assign_variables(&res_variable, $1, $3);
                                }

                                if (SUCCESS(err)) {
                                  err = insert(&root, res_variable);
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                /*print_variable($$);*/

                                // Cleanup
                                if (FAILED(err)) {
                                  delete_variable(&res_variable);
                                }

                                try_delete_variable(&$1);
                                try_delete_variable(&$3);
                              }
  | var_add                   {
                                $$ = $1;
                              }

var_add:
  var_add '+' var_mul         {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = sum_variables(
                                    &res_variable, $1, $3, '+'
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                                try_delete_variable(&$1);
                                try_delete_variable(&$3);
                              }
  | var_add '-' var_mul       {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = sum_variables(
                                    &res_variable, $1, $3, '-'
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                                try_delete_variable(&$1);
                                try_delete_variable(&$3);
                              }
  | var_mul                   {
                                $$ = $1;
                              }

var_mul:
  var_mul '*' var_neg         {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = mul_variables(&res_variable, $1, $3);
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                                try_delete_variable(&$1);
                                try_delete_variable(&$3);
                              }
  | var_mul var_pow           {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = mul_variables(&res_variable, $1, $2);
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                                try_delete_variable(&$1);
                                try_delete_variable(&$2);
                              }
  | var_neg                   {
                                $$ = $1;
                              }

var_neg:
  '-' var_neg %prec UMINUS    {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = neg_variable(&res_variable, $2);
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                                try_delete_variable(&$2);
                              }
  | var_pow                   {
                                $$ = $1;
                              }

var_pow:
  var '^' var_pow             {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = pow_variable(&res_variable, $1, $3);
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                                try_delete_variable(&$1);
                                try_delete_variable(&$3);
                              }
  | var                       {
                                $$ = $1;
                              }

var:
  '$' VAR_NAME                {
                                // Try to find variable in tree
                                // If not found create new variable with
                                // NULL polynomial
                                // Init
                                variable_t* res_variable = NULL;
                                variable_name_t* var_name = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = create_variable_name(
                                    &var_name, $2.buffer, $2.length
                                  );
                                }

                                if (SUCCESS(err)) {
                                  err = find_variable_by_name(
                                    root, var_name, &res_variable
                                  );
                                }

                                if (err == ERROR_INVALID_TREE) {
                                  err = create_variable(
                                    &res_variable, var_name, NULL
                                  );
                                } else { delete_variable_name(&var_name); }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                              }
  | '(' var_eq ')'            {
                                $$ = $2;
                              }
  | mono                      {
                                // Init
                                variable_t* res_variable = NULL;

                                // Perform an action
                                if (SUCCESS(err)) {
                                  err = create_variable(
                                    &res_variable, NULL, $1
                                  );
                                }

                                $$ = (SUCCESS(err)) ? res_variable : NULL;

                                // Cleanup
                              }


%%
