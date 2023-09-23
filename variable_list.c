#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable_list.h"

variable_t* allocate_variable(const int64_t __var_name_len);
void deallocate_variable(variable_t* __var);
void _print_variable(variable_t* __var);
void _print_variable_list(variable_list_t* __node);

variable_list_t* find_variable_list(
  variable_list_t* __head,
  variable_t* __var
);

variable_list_t* var_list = NULL;

variable_t* get_next_var() {
  static int64_t __var_idx = 1;
  char __var_name[32] = { 0x00 };
  int64_t __var_name_len = 0;

  __var_name_len = snprintf(__var_name, 32, "%ld", __var_idx);
  ++__var_idx;

  return create_variable(__var_name, __var_name_len);
}

void print_variable(variable_t* __var) {
  variable_list_t* __node = NULL;

  __node = find_variable_list(var_list, __var);
  _print_variable_list(__node);
}

void _print_variable(variable_t* __var_name) {
  printf("Variable name: %s\t Length: %ld\n",
    __var_name->name, __var_name->length
  );
}

variable_t* create_variable(
  const char* __var_name,
  const int64_t __var_name_len
) {
  variable_t* __new_var = allocate_variable(__var_name_len);

  memcpy(__new_var->name, __var_name, __var_name_len);
  __new_var->length = __var_name_len;

  return __new_var;
}

variable_t* allocate_variable(const int64_t __var_name_len) {
  variable_t* __new_var = NULL;

  __new_var = (variable_t*)calloc(1, sizeof(variable_t));
  if (__new_var == NULL) { return NULL; }

  __new_var->name = (char*)calloc(__var_name_len + 1, sizeof(char));
  if (__new_var->name == NULL) {
    free(__new_var);
    return NULL;
  }

  return __new_var;
}

void deallocate_variable(variable_t* __var_name) {
  free(__var_name->name);
  free(__var_name);
  __var_name = NULL;
}

inline void _print_variable_list(variable_list_t* __node) {
  if (__node == NULL) { return; }
  _print_variable(__node->var);
  _print_polynomial(__node->polynom);
}

void add_variable_list(
  variable_list_t** __head,
  variable_t* __var,
  polynomial_t* __polynom
) {
  variable_list_t* __tmp_node = NULL;
  variable_list_t* __new_node =
    (variable_list_t*)calloc(1, sizeof(variable_list_t));

  if (__new_node == NULL) { return; } 

  if (__var == NULL) { __var = get_next_var(); }
  __new_node->var = __var;
  __new_node->polynom = __polynom;

  if (*__head == NULL) { *__head = __new_node; }
  else {
    __tmp_node = find_variable_list(*__head, __var);

    // Remove if already exists 
    if (__tmp_node != NULL) { remove_variable_list(__head, __tmp_node); }

    if (*__head == NULL) { *__head = __new_node; }
    else {
      __tmp_node = *__head;
      while (__tmp_node->next != NULL) { __tmp_node = __tmp_node->next; }
      __tmp_node->next = __new_node;
    }
  }
}

variable_list_t* find_variable_list(
  variable_list_t* __head,
  variable_t* __var
) {
  while (__head != NULL) {
    if (__head->var->length == __var->length
         && strcmp(__head->var->name, __var->name) == 0) {
      break;
    }
    __head = __head->next;
  }

  return __head;
}

void remove_variable_list(
  variable_list_t** __head,
  variable_list_t* __node
) {
  variable_list_t* __tmp_node = NULL;
  variable_list_t* __node_to_remove = NULL;

  if (__head == NULL || *__head == NULL || __node == NULL) { return; }

  __tmp_node = *__head;

  if (__tmp_node == __node) {
    __node_to_remove = __tmp_node;
    *__head = (*__head)->next;
  } else {
    while (__tmp_node->next != __node) {
      printf("%p - %p", __tmp_node, __node);
      __tmp_node = __tmp_node->next;
      printf("%p - %p", __tmp_node, __node);
    }

    __node_to_remove = __tmp_node->next;
    __tmp_node->next = __node_to_remove->next;
  }

  deallocate_variable(__node_to_remove->var);
  deallocate_polynomial(__node_to_remove->polynom);
  free(__node_to_remove);
}

void remove_all_variables_list(variable_list_t** __head) {
  if (__head == NULL || *__head == NULL) { return; }
  while (*__head != NULL) { remove_variable_list(__head, *__head); }
}
