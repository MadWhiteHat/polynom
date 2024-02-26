#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable_list.h"

variable_t* allocate_variable(const int64_t var_name_len);
void deallocate_variable(variable_t* var);
void _print_variable(variable_t* var);
void _print_variable_list(variable_list_t* node);

variable_list_t* find_variable_list(
  variable_list_t* head,
  variable_t* var
);

variable_list_t* var_list = NULL;

variable_t* get_next_var() {
  static int64_t var_idx = 1;
  char var_name[32] = { 0x00 };
  int64_t var_name_len = 0;

  var_name_len = snprintf(var_name, 32, "%ld", var_idx);
  ++var_idx;

  return create_variable(var_name, var_name_len);
}

void print_variables_list(variable_list_t* head) {
  while (head != NULL) {
    _print_variable_list(head);
    head = head->next;
  }
  printf("NULL\n");
}

void print_variable(variable_t* var) {
  variable_list_t* node = NULL;

  node = find_variable_list(var_list, var);
  _print_variable_list(node);
}

void _print_variable(variable_t* var_name) {
  printf("Variable name: %s\t Length: %ld\n",
    var_name->name, var_name->length
  );
}

variable_t* create_variable(
  const char* var_name,
  const int64_t var_name_len
) {
  variable_t* new_var = allocate_variable(var_name_len);

  memcpy(new_var->name, var_name, var_name_len);
  new_var->length = var_name_len;

  return new_var;
}

variable_t* allocate_variable(const int64_t var_name_len) {
  variable_t* new_var = NULL;

  new_var = (variable_t*)calloc(1, sizeof(variable_t));
  if (new_var == NULL) { return NULL; }

  new_var->name = (char*)calloc(var_name_len + 1, sizeof(char));
  if (new_var->name == NULL) {
    free(new_var);
    return NULL;
  }

  return new_var;
}

void deallocate_variable(variable_t* var_name) {
  free(var_name->name);
  free(var_name);
  var_name = NULL;
}

inline void _print_variable_list(variable_list_t* node) {
  if (node == NULL) { return; }
  _print_variable(node->var);
  print_polynomial(node->polynom);
}

void add_variable_list(
  variable_list_t** head,
  variable_t* var,
  polynomial_t* polynom
) {
  variable_list_t* tmp_node = NULL;
  variable_list_t* new_node =
    (variable_list_t*)calloc(1, sizeof(variable_list_t));

  if (new_node == NULL) { return; } 

  if (var == NULL) { var = get_next_var(); }
  new_node->var = var;
  new_node->polynom = polynom;

  if (*head == NULL) { *head = new_node; }
  else {
    tmp_node = find_variable_list(*head, var);

    // Remove if already exists 
    if (tmp_node != NULL) { remove_variable_list(head, tmp_node); }

    if (*head == NULL) { *head = new_node; }
    else {
      tmp_node = *head;
      while (tmp_node->next != NULL) { tmp_node = tmp_node->next; }
      tmp_node->next = new_node;
    }
  }
}

variable_list_t* find_variable_list(
  variable_list_t* head,
  variable_t* var
) {
  while (head != NULL) {
    if (head->var->length == var->length
         && strcmp(head->var->name, var->name) == 0) {
      break;
    }
    head = head->next;
  }

  return head;
}

void remove_variable_list(
  variable_list_t** head,
  variable_list_t* node
) {
  variable_list_t* tmp_node = NULL;
  variable_list_t* node_to_remove = NULL;

  if (head == NULL || *head == NULL || node == NULL) { return; }

  tmp_node = *head;

  if (tmp_node == node) {
    node_to_remove = tmp_node;
    *head = (*head)->next;
  } else {
    while (tmp_node->next != node) {
      printf("%p - %p", tmp_node, node);
      tmp_node = tmp_node->next;
      printf("%p - %p", tmp_node, node);
    }

    node_to_remove = tmp_node->next;
    tmp_node->next = node_to_remove->next;
  }

  deallocate_variable(node_to_remove->var);
  deallocate_polynomial(node_to_remove->polynom);
  free(node_to_remove);
}

void remove_all_variables_list(variable_list_t** head) {
  if (head == NULL || *head == NULL) { return; }
  while (*head != NULL) { remove_variable_list(head, *head); }
}
