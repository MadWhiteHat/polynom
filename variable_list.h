#ifndef _VARIABLE_LIST_H
#define _VARIABLE_LIST_H

#include <stdio.h>
#include <stdint.h>

#include "polynom.h"

typedef struct variable {
  char* name;
  int64_t length;
} variable_t;

extern variable_t* create_variable(
  const char* __var_name,
  const int64_t __var_name_len
);

typedef struct variable_list {
  variable_t* var;
  polynomial_t* polynom;
  struct variable_list* next;
} variable_list_t;

extern variable_list_t* var_list;

extern void add_variable_list (
  variable_list_t** __head,
  variable_t* __var,
  polynomial_t* __polynom
);
extern void remove_variable_list(
  variable_list_t** __head,
  variable_list_t* __node
);
extern void remove_all_variables_list(variable_list_t** __head);

extern void print_variable(variable_t* __var);

#endif // _VARIABLE_LIST_H
