#include <stdio.h>
#include <string.h>

#include "variable_tree.h"

// TO DO:
// 1. Связать код с правилами для переменных

void
yyerror(const char* msg) { puts(msg); }

int main(int argc __attribute__((unused)), char** argv __attribute__((unused))) {
  char name[32] = { 0x00 };
  for (int i = 0; i < 10; ++i) {
    snprintf(name, 32, "test_name%d", i);
    variable_name_t* tmp_name = create_variable_name(name, strlen(name));
    polynomial_t* tmp_poly = create_polynomial(i + 1, 'x', 10 - i - 1);
    variable_t* tmp_var = create_variable(tmp_name, tmp_poly);
    root = insert(root, tmp_var);
  }

  print_tree(root);

  variable_name_t* var_name = create_variable_name("test_name5", strlen("test_name5"));
  variable_t* var = find_variable(root, var_name);

  print_variable(var);

  delete_tree(root);
  root = NULL;

  return 0;
}
