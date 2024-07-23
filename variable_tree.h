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

int
create_node(tree_node_t** this, variable_t* var);

int
insert(tree_node_t** this, variable_t* var);

tree_node_t*
rotate_right(tree_node_t* this);

tree_node_t*
rotate_left(tree_node_t* this);

void
print_tree(tree_node_t* this);

void
delete_tree(tree_node_t* this);

int
find_variable_by_name(
  tree_node_t* this,
  variable_name_t* var_name,
  variable_t** var
);

void
print_variable_by_name(tree_node_t* this, variable_name_t* var_name);

#endif // _VARIABLE_TREE_H
