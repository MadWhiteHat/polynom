#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable_tree.h"
#include "utility.h"

tree_node_t* root = NULL;

int32_t
height(tree_node_t* node) { return (node) ? node->height : 0; }

int32_t
get_balance(tree_node_t* node) {
  if (node == NULL) { return 0; }
  return height(node->left) - height(node->right);
}

int
create_node(tree_node_t** this, variable_t* var) {
  tree_node_t* new_node = (tree_node_t*)calloc(1, sizeof(tree_node_t));
  if (new_node == NULL) { return ERROR_MEMORY_ALLOCATION; }

  new_node->variable = var;
  new_node->right = NULL;
  new_node->left = NULL;
  new_node->height = 1;

  *this = new_node;
  return ERROR_SUCCESS;
}

int
insert(tree_node_t** this, variable_t* var) {
  int err = ERROR_SUCCESS;
  if (*this == NULL) { return create_node(this, var); }

  int32_t cmp_res = compare_variables(var, (*this)->variable);
  if (cmp_res == 0) {
    variable_t* tmp = (*this)->variable;
    (*this)->variable = var;
    delete_variable(&tmp);
    // no need to balance after inplace inserion
  } else {
    if (cmp_res < 0) {
      err = insert(&((*this)->left), var);
      if (FAILED(err)) { return err; }
    } else {
      err = insert(&((*this)->right), var);
      if (FAILED(err)) { return err; }
    }

    (*this)->height = 1 + MAX(height((*this)->left), height((*this)->right));

    int32_t balance = get_balance(*this);
    int32_t cmp_left = 0;
    int32_t cmp_right = 0;

    if ((*this)->left) {
      cmp_left = compare_variables(var, (*this)->left->variable);
    }

    if ((*this)->right) {
      cmp_right = compare_variables(var, (*this)->right->variable);
    }

    // LL case
    if (balance > 1 && cmp_left < 0) {
      *this = rotate_right(*this);
      return ERROR_SUCCESS;
    }
    // RR case
    if (balance < -1 && cmp_right > 0) {
      *this = rotate_left(*this);
      return ERROR_SUCCESS;
    }
    // LR case
    if (balance > 1 && cmp_left > 0) {
      (*this)->left = rotate_left((*this)->left);
      *this = rotate_right(*this);
      return ERROR_SUCCESS;
    }
    // RL case
    if (balance < -1 && cmp_right < 0) {
      (*this)->right = rotate_left((*this)->right);
      *this = rotate_left(*this);
      return ERROR_SUCCESS;
    }
  }

  return ERROR_SUCCESS;
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
  if (node == NULL) { return; }
  print_tree(node->left);
  print_variable(node->variable);
  DEBUG_PRINT("Node height: %d\n", node->height);
  print_tree(node->right);
}

void
delete_tree(tree_node_t* node) {
  if (node == NULL) { return; }
  delete_tree(node->left);
  delete_tree(node->right);
  delete_variable(&(node->variable));
  free(node);
}

int
find_variable_by_name(
  tree_node_t* this,
  variable_name_t* var_name,
  variable_t** var
) {
  int err = ERROR_SUCCESS;

  if (this == NULL) { return ERROR_INVALID_TREE; }
  if (var_name == NULL) { return ERROR_INVALID_VARIABLE_NAME; }

  err = is_valid_variable(this->variable);
  if (FAILED(err)) { return err; }

  int32_t cmp_var_names = compare_variable_names(
    this->variable->name, var_name
  );

  if (cmp_var_names == 0) {
    *var = this->variable;
    return ERROR_SUCCESS;
  } else if (cmp_var_names > 0) {
    return find_variable_by_name(this->left, var_name, var);
  } else { return find_variable_by_name(this->right, var_name, var); }
}

void
print_variable_by_name(tree_node_t* this, variable_name_t* var_name) {
  int err = ERROR_SUCCESS;
  variable_t* var = NULL;

  err = find_variable_by_name(this, var_name, &var);
  printf("Error code: %d\n", err);
  if (SUCCESS(err)) { print_variable(var); }
}
