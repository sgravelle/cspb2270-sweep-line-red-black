/* Note: refer to the header file (RBT.h) for documentation of each method. */

#include "RBT.h"

RBT::RBT() {
  // Constructor
  root = new rbt_node*;
  *root = NULL;
}

RBT::~RBT() {}

rbt_node* RBT::init_node(int data) {
  rbt_node* ret(new rbt_node);
  ret->data = data;
  ret->left = nullptr;
  ret->right = nullptr;
  ret->is_red = false;
  return ret;
}

// Helper function for rotating, 1 of 2
bool set_child(rbt_node* parent, string which_child, rbt_node* child) {
  if (which_child != "left" && which_child != "right"){
    return false;
  }
  if (which_child == "left"){
    parent->left = child;
  }
  else{
    parent->right = child;
  }
  return true;
}

// Helper function for rotating, 2 of 2
bool replace_child(rbt_node* parent, rbt_node* current_child, rbt_node* new_child) {
  if (parent->left == current_child){
    return set_child(parent, "left", new_child);
  }
  else if (parent->right == current_child){
    return set_child(parent, "right", new_child);
  }
  return false;
}

// Rotate left
void RBT::rotate_left(rbt_node* node){
  rbt_node* right_left_child = (node->right)->left;
  if (get_parent(node) != nullptr){
    replace_child(get_parent(node), node, node->right);
  }
  else { // node is root
    set_root(&(node->right));
  }
  set_child(node->right, "left", node);
  set_child(node, "right", right_left_child);
}

// Rotate right
void RBT::rotate_right(rbt_node* node){
  rbt_node* left_right_child = (node->left)->right;
  if (get_parent(node) != nullptr){
    replace_child(get_parent(node), node, node->left);
  }
  else { // node is root
    set_root(&(node->left));
  }
  set_child(node->left, "right", node);
  set_child(node, "left", left_right_child);  
}

void RBT::bst_insert(rbt_node* new_node) {
  // Special case: empty tree (root points to null)
  if (*root == nullptr){
    *root = new_node;
    return;
  }
  // Traverse binary tree in order until reaching the largest value that is smaller than new_node
  // then insert new_node there
  rbt_node* current_node = *root;
  while (current_node != nullptr){
    if (new_node->data < current_node->data){
      if(current_node->left == nullptr){
        current_node->left = new_node;
        return;
      }
      else{
        current_node = current_node->left;
      }
    }
    else{
      if (current_node->right == nullptr){
        current_node->right = new_node;
        return;
      }
      else{
        current_node = current_node->right;
      }
    }
  }
  return;
}

// Gramps and uncle helper functions
/*
RBTreeGetGrandparent(node) {
   if (node⇢parent == null)
      return null
   return node⇢parent⇢parent
}

RBTreeGetUncle(node) {
   grandparent = null
   if (node⇢parent != null)
      grandparent = node⇢parent⇢parent
   if (grandparent == null)
      return null
   if (grandparent⇢left == node⇢parent)
      return grandparent⇢right
   else
      return grandparent⇢left
}
*/

void RBT::balance_tree(rbt_node* node) {
/*
  if (node⇢parent == null) {
   node⇢color = black
   return
}
if (node⇢parent⇢color == black) {
   return
}
parent = node⇢parent
   grandparent = RBTreeGetGrandparent(node)
   uncle = RBTreeGetUncle(node)
   if (uncle != null && uncle⇢color == red) {
      uncle⇢color = black
      parent⇢color = black
      grandparent⇢color = red
      RBTreeBalance(tree, grandparent)
      return
   }
   if (node == parent⇢right &&
      parent == grandparent⇢left) {
      RBTreeRotateLeft(tree, parent)
      node = parent
      parent = node⇢parent
   }
   else if (node == parent⇢left &&
      parent == grandparent⇢right) {
      RBTreeRotateRight(tree, parent)
      node = parent
      parent = node⇢parent
   }
   parent⇢color = black
   grandparent⇢color = red
   if (node == parent⇢left) {
      RBTreeRotateRight(tree, grandparent)
   }
   else {
      RBTreeRotateLeft(tree, grandparent)
   }
*/
}


void RBT::insert(rbt_node* new_node) {
  bst_insert(new_node);
  new_node->is_red = true;
  balance_tree(new_node);
}

void RBT::insert_data(int data) {
  rbt_node* new_node = init_node(data);
  insert(new_node);
}

void RBT::remove(int data) {
  if (!contains(*root, data)){
    return;
  }
  else{
    rbt_node* doomed_node = get_node(*root, data);
    // Now do the rest

  }
}

bool RBT::contains(rbt_node* subt, int data) {
  return get_node(subt, data) != nullptr;
}

rbt_node* RBT::get_node(rbt_node* subt, int data) {
  while (subt != nullptr){
    if (subt->data == data){ return subt;}
    else{
      if (subt->data > data){
        subt = subt->left;
      }
      else{
        subt = subt->right;
      }
    }
  }
  return subt;
}

rbt_node* RBT::get_parent(rbt_node* child){
  if (child == nullptr){ return nullptr; }
  rbt_node* ret = *root;
  while (ret != nullptr && ret->left != child && ret->right != child){
    if (child->data < ret->data){
      ret = ret->left;
    }
    else{
      ret = ret->right;
    }
  }
  return ret;
}

int RBT::size(rbt_node* subt) {
  if (subt == nullptr){
    return 0;
  }
  else {
    int count = 0;
    count += size(subt->left);
    count ++;
    count += size(subt->right);
    return count;
  }
}

void RBT::to_vector(rbt_node* subt, vector<int>& vec) {
  if (subt == nullptr){ return; }
  else{
    to_vector(subt->left, vec);
    vec.push_back(subt->data);
    to_vector(subt->right, vec);
  }
}

rbt_node* RBT::get_root() {
  if (*root == NULL)
    return NULL;
  return *root;
}

void RBT::set_root(rbt_node** new_root) {
  root = new_root;
}
