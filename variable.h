#ifndef _VARIABLE_H
#define _VARIABLE_H

#include <stdint.h>

#include "polynom.h"

//-------------------------------Variable name--------------------------------//

typedef struct variable_name {
  char* buffer;
  uint32_t length;
} variable_name_t;

// Constructor
variable_name_t*
create_variable_name(char* var_name, uint32_t var_length);

// Destructor
void
delete_variable_name(variable_name_t* var_name);

// Copy constructor
variable_name_t*
copy_variable_name(variable_name_t* var_name);

// Operations
void
print_variable_name(variable_name_t* var_name);

int32_t
compare_variable_names(variable_name_t* lhs, variable_name_t* rhs);

//----------------------------------Variable----------------------------------//

typedef struct variable {
  variable_name_t* name;
  polynomial_t* polynomial;
} variable_t;

// Constructor
variable_t*
create_variable(variable_name_t* name, polynomial_t* polynom);

// Destructor
void
delete_variable(variable_t* var);

// Operations
void
is_valid_variable_operation(variable_t* lhs, variable_t* rhs);

void
is_valid_variable(variable_t* var);

void
is_persistent_variable(variable_t* var);

void
print_variable(variable_t* var);

int32_t
compare_variables(variable_t* lhs, variable_t* rhs);

// Deletes only temporary variables (without names)
void
try_delete_variable(variable_t* var);

variable_t*
sum_variables(variable_t* lhs, variable_t* rhs, const char action);

variable_t*
mul_variables(variable_t* lhs, variable_t* rhs);

variable_t*
neg_variable(variable_t* var);

variable_t*
pow_variable(variable_t* var, int64_t power);

#endif // _VARIABLE_H
