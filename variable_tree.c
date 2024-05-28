#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable_tree.h"

tree_node_t* root = NULL;

tree_node_t* create_node(variable_t* variable) {
  tree_node_t* new_node = (tree_node_t*)calloc(1, sizeof(tree_node_t));

  new_node->variable = variable;
  new_node->right = NULL;
  new_node->left = NULL;
  new_node->height = 1;

  return new_node;
}

int32_t
height(tree_node_t* node) { return (node) ? node->height : 0; }

int32_t
get_balance(tree_node_t* node) {
  if (node == NULL) { return 0; }
  return height(node->left) - height(node->right);
}

tree_node_t*
insert(tree_node_t* node, variable_t* variable) {
  if (node == NULL) { return create_node(variable); }

  int32_t cmp_res = compare_variables(variable, node->variable);
  if (cmp_res == 0) {
    variable_t* tmp = node->variable;
    node->variable = variable;
    delete_variable(tmp);
    // no need to balance after inplace inserion
  } else {
    if (cmp_res < 0) { node->left = insert(node->left, variable); }
    else { node->right = insert(node->right, variable); }

    node->height = 1 + MAX(height(node->left), height(node->right));

    int32_t balance = get_balance(node);
    int32_t cmp_left = 0;
    if (node->left) {
      cmp_left = compare_variables(variable, node->left->variable);
    }
    int32_t cmp_right = 0;
    if (node->right) {
      cmp_right = compare_variables(variable, node->right->variable);
    }

    // LL
    if (balance > 1 && cmp_left < 0) {
      return rotate_right(node);
    }
    // RR
    if (balance < -1 && cmp_right > 0) {
      return rotate_left(node);
    }
    // LR
    if (balance > 1 && cmp_left > 0) {
      node->left = rotate_left(node->left);
      return rotate_right(node);
    }
    // RL
    if (balance < -1 && cmp_right < 0) {
      node->right = rotate_right(node->right);
      return rotate_left(node);
    }
  }

  return node;
}

tree_node_t*
rotate_right(tree_node_t* node) {
  tree_node_t* left = node->left;
  tree_node_t* left_right = left->right;

  left->right = node;
  node->left = left_right;

  node->height = MAX(height(node->left), height(node->right)) + 1;
  left->height = MAX(height(left->left), height(left->right)) + 1;

  return left;
}

tree_node_t*
rotate_left(tree_node_t* node) {
  tree_node_t* right = node->right;
  tree_node_t* right_left = right->left;

  right->left = node;
  node->right = right_left;

  node->height = MAX(height(node->left), height(node->right)) + 1;
  right->height = MAX(height(right->left), height(right->right)) + 1;

  return right;
}

void
print_tree(tree_node_t* node) {
  if (!node) { return; }
  print_tree(node->left);
  print_variable(node->variable);
  printf("Node height: %d\n", node->height);
  print_tree(node->right);
}

void
delete_tree(tree_node_t* node) {
  if (!node) { return; }
  delete_tree(node->left);
  delete_tree(node->right);
  node->left = node->right = NULL;
  delete_variable(node->variable);
  free(node);
}

variable_t*
find_variable_by_name(tree_node_t* node, variable_name_t* var_name) {
  if (!node || !var_name) { return NULL; }
  if (!node->variable) { return NULL; } 

  int32_t cmp_var_names = compare_variable_names(
    node->variable->name, var_name
  );

  if (cmp_var_names == 0) { return node->variable; }
  else if (cmp_var_names > 0) {
    return find_variable_by_name(node->left, var_name);
  } else { return find_variable_by_name(node->right, var_name); }
}

void
print_variable_by_name(variable_name_t* var_name) {
  variable_t* var = find_variable_by_name(root, var_name);

  print_variable(var);
}
