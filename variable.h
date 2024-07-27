#ifndef _VARIABLE_H
#define _VARIABLE_H

#include <stdint.h>

#include "polynom.h"

#define VAR_NAME_INFO_BUFFER_SIZE 128

//-------------------------------Variable name--------------------------------//

typedef struct variable_name_info {
  char buffer[VAR_NAME_INFO_BUFFER_SIZE];
  uint32_t length;
} variable_name_info_t;

typedef struct variable_name {
  char* buffer;
  uint32_t length;
} variable_name_t;

// Constructor
int
create_variable_name(
  variable_name_t** this,
  char* var_name,
  uint32_t var_length
);

// Destructor
void
delete_variable_name(variable_name_t** this);

// Copy constructor
int
copy_variable_name(variable_name_t** this, variable_name_t* other);

// Operations
void
print_variable_name(variable_name_t* this);

int32_t
compare_variable_names(variable_name_t* lhs, variable_name_t* rhs);

//----------------------------------Variable----------------------------------//

typedef struct variable {
  variable_name_t* name;
  polynomial_t* polynomial;
} variable_t;

// Constructor
int
create_variable(
  variable_t** this,
  variable_name_t* name,
  polynomial_t* polynom
);

// Destructor
void
delete_variable(variable_t** this);

// Deletes only temporary variables (without names)
void
try_delete_variable(variable_t** this);

// Operations
int
is_valid_variable_operation(variable_t* lhs, variable_t* rhs);

int
is_valid_variable(variable_t* this);

int
is_initialized_variable(variable_t* this);

int
is_persistent_variable(variable_t* this);

void
print_variable(variable_t* this);

int32_t
compare_variables(variable_t* lhs, variable_t* rhs);

int
assign_variables(variable_t** this, variable_t* lhs, variable_t* rhs);

int
sum_variables(
  variable_t** this,
  variable_t* lhs,
  variable_t* rhs,
  const char action
);

int
mul_variables(variable_t** this, variable_t* lhs, variable_t* rhs);

int
neg_variable(variable_t**this, variable_t* other);

int
pow_variable(variable_t**this, variable_t* lhs, variable_t* rhs);

#endif // _VARIABLE_H
