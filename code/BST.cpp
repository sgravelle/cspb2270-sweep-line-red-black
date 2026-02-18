/* Note: refer to the header file (BST.h) for documentation of each method. */

#include "BST.h"

BST::BST() {
  // Here is one way to implement the constructor. Keep or change it, up to you.
  root = new bst_node*;
  *root = NULL;
}

BST::~BST() {}

bst_node* BST::init_node(int data) {
  bst_node* ret(new bst_node);
  ret->data = data;
  ret->left = nullptr;
  ret->right = nullptr;
  return ret;
}

void BST::insert(bst_node* new_node) {
  // Special case: empty tree (root points to null)
  if (*root == nullptr){
    *root = new_node;
    return;
  }
  // Traverse binary tree in order until reaching the largest value that is smaller than new_node
  // then insert new_node there
  bst_node* current_node = *root;
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

void BST::insert_data(int data) {
  bst_node* new_node = init_node(data);
  insert(new_node);
}

void BST::remove(int data) {
  if (!contains(*root, data)){
    return;
  }
  else{
    bst_node* doomed_node = get_node(*root, data);
    // remove node with no children
    if (doomed_node->left == nullptr && doomed_node->right == nullptr){
      if (get_parent(doomed_node)->left == doomed_node){
        get_parent(doomed_node)->left = nullptr;
      }
      else{
        get_parent(doomed_node)->right = nullptr;
      }
      delete doomed_node;
      return;
    }
    // remove node with exactly one child on the right
    else if (doomed_node != *root && doomed_node->left == nullptr){
      bst_node* child = doomed_node->right;
      if (doomed_node == *root){
        *root = doomed_node->right;
        delete doomed_node;
        return;
      }
      else if (get_parent(doomed_node)->left == doomed_node){
        get_parent(doomed_node)->left = child;
      }
      else{
        get_parent(doomed_node)->right = child;
      }
    delete doomed_node;
    return;
    }
    // remove node with exactly one child on the left
    else if (doomed_node->right == nullptr){
      bst_node* child = doomed_node->left;
      if (doomed_node == *root){
        *root = doomed_node->left;
        delete doomed_node;
        return;
      }
      else if (get_parent(doomed_node)->left == doomed_node){
        get_parent(doomed_node)->left = child;
      }
      else{
        get_parent(doomed_node)->right = child;
      }
    delete doomed_node;
    return;
    }
    // remove non-root node with two children
    else if (doomed_node != *root){
      //  Replace it with (arbitrarily) the predecessor.
      bst_node* buddy = doomed_node->left;
      while (buddy->right != nullptr){
        buddy = buddy->right;
      }
      doomed_node->data = buddy->data;
      // delete old buddy spot
      // hard coding it because the remove function takes data as input not node oops
      // remove buddy with no children
      if (buddy->left == nullptr && buddy->right == nullptr){
        if (get_parent(buddy)->left == buddy){
          get_parent(buddy)->left = nullptr;
        }
        else{
          get_parent(buddy)->right = nullptr;
        }
        delete buddy;
        return;
      }
      // remove buddy with exactly one child on the right
      else if (buddy != *root && buddy->left == nullptr){
        bst_node* child = buddy->right;
        if (buddy == *root){
          *root = buddy->right;
          delete buddy;
          return;
        }
        else if (get_parent(buddy)->left == buddy){
          get_parent(buddy)->left = child;
        }
        else{
          get_parent(buddy)->right = child;
        }
      delete buddy;
      return;
      }
      // remove buddy with exactly one child on the left
      else if (buddy->right == nullptr){
        bst_node* child = buddy->left;
        if (buddy == *root){
          *root = buddy->left;
          delete buddy;
          return;
        }
        else if (get_parent(buddy)->left == buddy){
          get_parent(buddy)->left = child;
        }
        else{
          get_parent(buddy)->right = child;
        }
      delete buddy;
      return;
      }
    }
  }
}

bool BST::contains(bst_node* subt, int data) {
  return get_node(subt, data) != nullptr;
}

bst_node* BST::get_node(bst_node* subt, int data) {
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

bst_node* BST::get_parent(bst_node* child){
  if (child == nullptr){ return nullptr; }
  bst_node* ret = *root;
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

int BST::size(bst_node* subt) {
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

void BST::to_vector(bst_node* subt, vector<int>& vec) {
  if (subt == nullptr){ return; }
  else{
    to_vector(subt->left, vec);
    vec.push_back(subt->data);
    to_vector(subt->right, vec);
  }
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
