#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utility.h"
#include "variable.h"
#include "variable_tree.h"

//-------------------------------Variable name--------------------------------//

// Constructor
int
create_variable_name(
  variable_name_t** this,
  char* var_name,
  uint32_t var_length
) {
  variable_name_t* new_var_name = (variable_name_t*)calloc(
    1, sizeof(variable_name_t)
  );

  if (new_var_name == NULL) { return ERROR_MEMORY_ALLOCATION; }

  new_var_name->buffer = (char*)calloc(var_length + 1, sizeof(char));
  if (new_var_name->buffer == NULL) {
    free(new_var_name);
    return ERROR_MEMORY_ALLOCATION;
  }

  memcpy(new_var_name->buffer, var_name, var_length);
  new_var_name->length = var_length;

  *this = new_var_name;
  return ERROR_SUCCESS;
}

// Destructor
void
delete_variable_name(variable_name_t** this) {
  if (*this) {
    if ((*this)->buffer) { free((*this)->buffer); }
    free(*this);

    *this = NULL;
  }
}

// Copy constructor
int
copy_variable_name(variable_name_t** this, variable_name_t* other) {
  return create_variable_name(this, other->buffer, other->length);
}

// Opertaions

void
print_variable_name(variable_name_t* this) {
  if (this == NULL) {
    printf("Variable name is NULL\n");
    return;
  }
  printf(
    "Variable name: %s\t Length: %d\n",
    (this->buffer) ? this->buffer : "NULL", this->length
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
int
create_variable(
  variable_t** this,
  variable_name_t* name,
  polynomial_t* polynomial
) {
  variable_t* new_var = (variable_t*)calloc(1, sizeof(variable_t));
  if (new_var == NULL) { return ERROR_MEMORY_ALLOCATION; }

  new_var->name = name;
  new_var->polynomial = polynomial;

  *this = new_var;

  return ERROR_SUCCESS;
}

// Destructor
void
delete_variable(variable_t** this) {
  if (*this) {
    delete_variable_name(&((*this)->name));
    delete_polynomial(&((*this)->polynomial));
    free(*this);

    *this = NULL;
  }
}

// Opertaions
int
is_valid_variable_operation(variable_t* lhs, variable_t* rhs) {
  int err = ERROR_SUCCESS;

  err = is_valid_variable(lhs);
  if (FAILED(err)) { return err; }

  err = is_valid_variable(rhs);
  if (FAILED(err)) { return err; }

  err = is_initialized_variable(lhs);
  if (FAILED(err)) { return err; }

  err = is_initialized_variable(rhs);
  if (FAILED(err)) { return err; }

  return is_valid_polynomial_operation(lhs->polynomial, rhs->polynomial);
}

int
is_valid_variable(variable_t* this) {
  // Null name and null polinomial at once is considered as invalid state

  if (this == NULL || (this->name == NULL && this->polynomial == NULL)) {
    print_error(SEMANTICS, "invalid variable");
    return ERROR_INVALID_VARIABLE;
  }

  return ERROR_SUCCESS;
}

int
is_initialized_variable(variable_t* this) {
  if (this->polynomial == NULL) {
    print_error(SEMANTICS, "uninitialized variable '%s'", this->name->buffer);
    return ERROR_UNINITIALIZED_VARIABLE;
  }

  return is_valid_polynomial(this->polynomial);
}

// Checks if isn't temporary
int
is_persistent_variable(variable_t* this) {
  int err = ERROR_SUCCESS;

  err = is_valid_variable(this);
  if (FAILED(err)) { return err; }

  if (this->name == NULL) {
    print_error(SEMANTICS, "assignment to rvalue");
    return ERROR_RVALUE_ASSIGNMENT;
  }

  return ERROR_SUCCESS;
}

void
print_variable(variable_t* this) {
  if (this == NULL) {
    printf("Variable is NULL\n");
    return;
  }
  print_variable_name(this->name);
  print_polynomial(this->polynomial);
}

int32_t
compare_variables(variable_t* lhs, variable_t* rhs) {
  if (lhs == rhs) { return 0; }
  if (lhs == NULL) { return -1; }
  else if (rhs == NULL) { return 1; }

  return compare_variable_names(lhs->name, rhs->name);
}

void
try_delete_variable(variable_t** this) {
  variable_t* tree_var = NULL;
  err = find_variable_by_name(root, (*this)->name, &tree_var);
  if (SUCCESS(err) && tree_var != *this) { delete_variable(this); }
}

int
assign_variables(variable_t** this, variable_t* lhs, variable_t* rhs) {
  int err = ERROR_SUCCESS;
  variable_t* res_variable = NULL;
  variable_name_t* res_variable_name = NULL;
  polynomial_t* res_variable_polynomial = NULL;

  err = is_persistent_variable(lhs);
  if (FAILED(err)) { return err; }

  err = is_valid_variable(rhs);
  if (FAILED(err)) { return err; }

  err = is_initialized_variable(rhs);
  if (FAILED(err)) { return err; }

  err = copy_variable_name(&res_variable_name, lhs->name);
  if (FAILED(err)) { return err; }

  err = copy_polynomial(&res_variable_polynomial, rhs->polynomial);
  if (FAILED(err)) {
    delete_variable_name(&res_variable_name);
    return err;
  }

  err = create_variable(
    &res_variable, res_variable_name, res_variable_polynomial
  );
  if (FAILED(err)) {
    delete_variable_name(&res_variable_name);
    delete_polynomial(&res_variable_polynomial);
    return err;
  }

  *this = res_variable;
  return ERROR_SUCCESS;
}

int
sum_variables(
  variable_t** this,
  variable_t* lhs,
  variable_t* rhs,
  const char action
) {
  int err = ERROR_SUCCESS;
  polynomial_t* res_polynomial = NULL;

  err = is_valid_variable_operation(lhs, rhs);
  if (FAILED(err)) { return err; }

  err = sum_polynomials(
    &res_polynomial, lhs->polynomial, rhs->polynomial, action
  );
  if (FAILED(err)) { return err; }

  // return temporary variable regargless of operands type
  return create_variable(this, NULL, res_polynomial);
}

int
mul_variables(variable_t** this, variable_t* lhs, variable_t* rhs) {
  int err = ERROR_SUCCESS;
  polynomial_t* res_polynomial = NULL;

  err = is_valid_variable_operation(lhs, rhs);
  if (FAILED(err)) { return err; }

  err = mul_polynomials(&res_polynomial, lhs->polynomial, rhs->polynomial);
  if (FAILED(err)) { return err; }

  // return temporary variable regargless of operands type
  return create_variable(this, NULL, res_polynomial);
}

int
neg_variable(variable_t**this, variable_t* other) {
  int err = ERROR_SUCCESS;
  polynomial_t* res_polynomial = NULL;

  err = is_valid_variable(other);
  if (FAILED(err)) { return err; }

  err = is_initialized_variable(other);
  if (FAILED(err)) { return err; }

  err = neg_polynomial(&res_polynomial, other->polynomial);
  if (FAILED(err)) { return err; }

  // return temporary variable regargless of operand type
  return create_variable(this, NULL, res_polynomial);
}

int
pow_variable(variable_t**this, variable_t* lhs, variable_t* rhs) {
  int err = ERROR_SUCCESS;
  polynomial_t* res_polynomial = NULL;

  err = is_valid_variable_operation(lhs, rhs);
  if (FAILED(err)) { return err; }

  err = pow_polynomial(&res_polynomial, lhs->polynomial, rhs->polynomial);
  if (FAILED(err)) { return err; }

  // return temporary variable regargless of operand type
  return create_variable(this, NULL, res_polynomial);
}
