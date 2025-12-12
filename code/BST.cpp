/* Note: refer to the header file (BST.h) for documentation of each method. */

#include "BST.h"

BST::BST() {
  // Here is one way to implement the constructor. Keep or change it, up to you.
  root = new bst_node*;
  *root = NULL;
}

BST::~BST() {}

bst_node* BST::init_node(int data) { return NULL; }

void BST::insert(bst_node* new_node) {
  // Your code here
}

void BST::insert_data(int data) {
  // Your code here
}

void BST::remove(int data) {
  // Your code here
}

bool BST::contains(bst_node* subt, int data) {
  // Your code here
  return false;
}

bst_node* BST::get_node(bst_node* subt, int data) {
  // Your code here
  return NULL;
}

int BST::size(bst_node* subt) {
  // Your code here
  return 0;
}

void BST::to_vector(bst_node* subt, vector<int>& vec) {
  // Your code here
}

bst_node* BST::get_root() {
  // This function is implemented for you
  if (*root == NULL)
    return NULL;
  return *root;
}

void BST::set_root(bst_node** new_root) {
  // This function is implemented for you
  root = new_root;
}
