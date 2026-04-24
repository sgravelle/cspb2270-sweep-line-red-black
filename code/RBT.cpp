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

rbt_node* RBT::find_gramps(rbt_node* node) {
  if (get_parent(node) == nullptr){
    return nullptr;
  }
  return get_parent(get_parent(node));
}

rbt_node* RBT::find_uncle(rbt_node* node) {
  rbt_node* grandparent = find_gramps(node);
  if (grandparent == nullptr){
    return nullptr;
  }
  if (grandparent->left == get_parent(node))
    return grandparent->right;
  else{
    return grandparent->left;
  }
}


void RBT::balance_tree(rbt_node* node) {
  if (get_parent(node) == nullptr) {
    node->is_red = false;
    return;
  }
  if (!(get_parent(node)->is_red)) {
    return;
  }
  rbt_node* parent = get_parent(node);
  rbt_node* grandparent = find_gramps(node);
  rbt_node* uncle = find_uncle(node);
  if (uncle != nullptr && uncle->is_red) {
    uncle->is_red = false;
    parent->is_red = false;
    grandparent->is_red = true;
    balance_tree(grandparent);
    return;
  }
  if (node == parent->right && parent == grandparent->left) {
    rotate_left(parent);
    node = parent;
    parent = get_parent(node);
  }
  else if (node == parent->left && parent == grandparent->right) {
    rotate_right(parent);
    node = parent;
    parent = get_parent(node);
  }
  parent->is_red = false;
  grandparent->is_red = true;
  if (node == parent->left) {
    rotate_right(grandparent);
  }
  else {
    rotate_left(grandparent);
  }
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
  rbt_node* node = get_node(*root, data);
  if (node->left != nullptr && node->right != nullptr){
    rbt_node* pre_node = get_predecessor(node);
    int pre_data = pre_node->data;
    remove(pre_data);
    node->data = pre_data;
    return;
  }
  if (!(node->is_red)){
    prepare_to_remove(node);
  }
  remove(node->data);
}

void RBT::prepare_to_remove(rbt_node* node){
  if (case1(node)){
    return;
  }
  rbt_node* sibling = get_sibling(node);
  if (case2(node, sibling)){
    sibling = get_sibling(node);
  }
  if (case3(node, sibling)){
    return;
  }
  if (case4(node, sibling)){
    return;
  }
  if (case5(node, sibling)){
    sibling = get_sibling(node);
  }
  if (case6(node, sibling)){
    sibling = get_sibling(node);
  }
  rbt_node* parent = get_parent(node);
  sibling->is_red = parent->is_red;
  parent->is_red = false;
  if (node == parent->left){
    (sibling->right)->is_red = false;
    rotate_left(parent);
  }
  else{
    (sibling->left)->is_red = false;
    rotate_right(parent);
  }
}

bool RBT::case1(rbt_node* node){
  if (node->is_red || get_parent(node) == nullptr){
    return true;
  }
  else{
    return false;
  }     
}

bool RBT::case2(rbt_node* node, rbt_node* sibling){
  if (sibling->is_red) {
    get_parent(node)->is_red = true;
    sibling->is_red = false;
    if (node == get_parent(node)->left){
      rotate_left(get_parent(node));
    }
    else{
      rotate_right(get_parent(node));
    }
    return true;
  }
  return false;
}

bool RBT::case3(rbt_node* node, rbt_node* sibling){
  if (get_parent(node)->is_red == false && both_child_black(sibling)) {
    sibling->is_red = true;
    prepare_to_remove(get_parent(node));
    return true;
  }
  return false;
}

bool RBT::case4(rbt_node* node, rbt_node* sibling){
  if (get_parent(node)->is_red && both_child_black(sibling)){
    get_parent(node)->is_red = false;
    sibling->is_red = true;
    return true;
  }
  return false;
}

bool RBT::case5(rbt_node* node, rbt_node* sibling){
  if (is_non_null_red(sibling->left) &&
  is_null_or_black(sibling->right) &&
  node == get_parent(node)->left){
    sibling->is_red = true;
    (sibling->left)->is_red = false;
    rotate_right(sibling);
    return true;
  }
  return false;
}

bool RBT::case6(rbt_node* node, rbt_node* sibling){
  if (is_non_null_red(sibling->right) &&
  is_null_or_black(sibling->left) &&
  node == get_parent(node)->right){
    sibling->is_red = true;
    (sibling->right)->is_red = false;
    rotate_left(sibling);
    return true;
  }
  return false;
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

rbt_node* RBT::get_sibling(rbt_node* node) {
  if (get_parent(node) != nullptr){
    if (node == get_parent(node)->left){
      return get_parent(node)->right;
    }
    return get_parent(node)->left;
  }
  return nullptr;
}

rbt_node* RBT::get_predecessor(rbt_node* node){
  node = node->left;
  while (node->right != nullptr){
    node = node->right;
  }
  return node;
}

bool RBT::both_child_black(rbt_node* node){
  rbt_node* left = node->left;
  rbt_node* right = node->right;
  if (left == nullptr){
    return !(right->is_red);
  }
  if (right == nullptr){
    return !(left->is_red);
  }
  return ((!(left->is_red)) && (!(right->is_red)));
}

bool RBT::is_non_null_red(rbt_node* node) {
  if (node == nullptr){
    return false;
  }
  return node->is_red;
}

bool RBT::is_null_or_black(rbt_node* node) {
  if (node == nullptr){
    return true;
  }
  return !(node->is_red);
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
