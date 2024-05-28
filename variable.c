#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "errors.h"
#include "variable.h"
#include "variable_tree.h"

//-------------------------------Variable name--------------------------------//

// Constructor
variable_name_t*
create_variable_name(char* var_name, uint32_t var_length) {
  variable_name_t* new_var_name = (variable_name_t*)calloc(
    1, sizeof(variable_name_t)
  );

  if (!new_var_name) { return NULL; }

  new_var_name->buffer = (char*)calloc(var_length + 1, sizeof(char));
  if (!new_var_name->buffer) {
    free(new_var_name);
    return NULL;
  }

  memcpy(new_var_name->buffer, var_name, var_length);
  new_var_name->length = var_length;

  return new_var_name;
}

// Destructor
void
delete_variable_name(variable_name_t* var_name) {
  if (var_name) {
    if (var_name->buffer) { free(var_name->buffer); }
    free(var_name);
  }
}

// Copy constructor
variable_name_t*
copy_variable_name(variable_name_t* var_name) {
  return create_variable_name(var_name->buffer, var_name->length);
}

// Opertaions

void
print_variable_name(variable_name_t* var_name) {
  if (!var_name) {
    printf("Variable name is NULL\n");
    return;
  }
  printf(
      "Variable name: %s\t Length: %d\n",
      (var_name->buffer) ? var_name->buffer : "NULL", var_name->length
  );
}

// If lhs == rhs return 0
// if lhs == NULL and rhs != NULL return 1
// if lhs != NULL and rhs == NULL return -1
// Otherwise return result of lexicographical comparison of variable names
int32_t
compare_variable_names(variable_name_t* lhs, variable_name_t* rhs) {
  if (lhs == rhs) { return 0; }
  if (lhs == NULL) { return -1; }
  else if (rhs == NULL) { return 1; }

  if (lhs->buffer == NULL) { return -(rhs->buffer != NULL); }
  else if (rhs->buffer == NULL) { return 1; }

  return strcmp(lhs->buffer, rhs->buffer);
}

//----------------------------------Variable----------------------------------//

// Constructor
variable_t*
create_variable(variable_name_t* name, polynomial_t* polynomial) {
  variable_t* new_var = (variable_t*)calloc(1, sizeof(variable_t));
  if (!new_var) { return NULL; }

  new_var->name = name;
  new_var->polynomial = polynomial;

  return new_var;
}

// Destructor
void delete_variable(variable_t* var) {
  if (var) {
    delete_variable_name(var->name);
    delete_polynomial(var->polynomial);
    free(var);
  }
}

// Opertaions
void is_valid_variable_operation(variable_t* lhs, variable_t* rhs) {
  is_valid_variable(lhs);
  is_valid_variable(rhs);
  is_valid_polynomial_operation(lhs->polynomial, rhs->polynomial);
}

void
is_valid_variable(variable_t* var) {
  if (!var) { 
    print_error(SEMANTICS, "invalid variable");
    exit(-1);
  }
}

// Checks if isn't temporary
void
is_persistent_variable(variable_t* var) {
  is_valid_variable(var);
  if (!var->name) {
    print_error(SEMANTICS, "assignment to rvalue");
    exit(-1);
  }
}

void
print_variable(variable_t* var) {
  if (!var) {
    printf("Variable is NULL\n");
    return;
  }
  print_variable_name(var->name);
  print_polynomial(var->polynomial);
}

int32_t
compare_variables(variable_t* lhs, variable_t* rhs) {
  if (lhs == rhs) { return 0; }
  if (lhs == NULL) { return -1; }
  else if (rhs == NULL) { return 1; }

  return compare_variable_names(lhs->name, rhs->name);
}

void
try_delete_variable(variable_t* var) {
  variable_t* tree_var = find_variable_by_name(root, var->name);
  if (tree_var != var) { delete_variable(var); }
}

variable_t*
sum_variables(variable_t* lhs, variable_t* rhs, const char action) {
  polynomial_t* res_polynomial = sum_polynomials(
    lhs->polynomial, rhs->polynomial, action
  );

  // return temporary variable regargless of operands type
  return create_variable(NULL, res_polynomial);
}

variable_t*
mul_variables(variable_t* lhs, variable_t* rhs) {
  polynomial_t* res_polynomial = mul_polynomials(
    lhs->polynomial, rhs->polynomial
  );

  // return temporary variable regargless of operands type
  return create_variable(NULL, res_polynomial);
}

variable_t*
neg_variable(variable_t* var) {
  polynomial_t* res_polynomial = neg_polynomial(var->polynomial);

  // return temporary variable regargless of operand type
  return create_variable(NULL, res_polynomial);
}

variable_t*
pow_variable(variable_t* var, int64_t power) {
  polynomial_t* res_polynomial = pow_polynomial(var->polynomial, power);

  // return temporary variable regargless of operand type
  return create_variable(NULL, res_polynomial);
}
