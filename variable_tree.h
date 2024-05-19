#ifndef _VARIABLE_TREE_H
#define _VARIABLE_TREE_H

#include <stdint.h>

#include "variable.h"

typedef struct tree_node {
  variable_t* variable;
  struct tree_node* left;
  struct tree_node* right;
  int32_t height;
} tree_node_t;

extern tree_node_t* root;

tree_node_t*
insert(tree_node_t* node, variable_t* name);

tree_node_t*
rotate_right(tree_node_t* node);

tree_node_t*
rotate_left(tree_node_t* node);

void
print_tree(tree_node_t* node);

void
delete_tree(tree_node_t* node);

variable_t*
find_variable_by_name(tree_node_t* node, variable_name_t* var_name);

void
print_variable_by_name(variable_name_t* var_name);

#endif // _VARIABLE_TREE_H
