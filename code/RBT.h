#ifndef RBT_H__
#define RBT_H__

#include <memory>
#include <string>
#include <vector>

using namespace std;

// rbt_node is the red-black tree node structure.
struct rbt_node {
  int data;
  bool is_red; // so false means it's black
  rbt_node* left;
  rbt_node* right;
};

// Red-black tree:
//
// From any subtree node t, the left subtree's data values must be
// less than t's data value. The right subtree's data values must be
// greater than or equal to t's data value.
class RBT {
public:
  // The constructor initializes class variables and pointers here if needed.
  // Set root to null.
  RBT();

  // deconstructor - use this to clean up all memory that the BST has allocated
  // but not returned with the 'delete' keyword.
  ~RBT();

  // init_node initializes a new rbt_node from the heap using the given
  // data, and two NULL children, and returns a pointer to it.
  // The node is initially colored black.
  rbt_node* init_node(int data);

  // rotate left at the given node
  void rotate_left(rbt_node* node);

  // rotate right at the given node
  void rotate_right(rbt_node* node);

  // bst_insert is the exact insert function for an unbalanced binary search tree.
  // It places the new_node in a proper location in the tree while obeying
  // the invariant. On return, root points to the root of the tree.
  void bst_insert(rbt_node* new_node);

  // balance_tree takes an unbalanced binary tree
  // Makes it balanced and recolors nodes to make it a valid red-black tree
  // Input is the node whose recent insertion has made the tree unbalanced
  void balance_tree(rbt_node* node);

  // insert first performs bst_insert,
  // then recolors and balances the tree
  void insert(rbt_node* new_node);

  // insert_data creates a new node with the given data value and inserts it
  // into the tree.
  void insert_data(int data);

  // This removes a node from the tree whose data value matches the input.
  // If no node in the tree contains the given data, this function has no effect.
  // Rebalances the tree and recolors nodes as needed.
  void remove(int data);

  // contains returns true if any node in the subtree pointed to by subt
  // contains the given data value, false otherwise.
  bool contains(rbt_node* subt, int data);

  // get_node searches through the subtree pointed to by subt for a node that
  // contains the given data value. If such a node is found, a pointer
  // to it is returned. Otherwise this function returns NULL.
  rbt_node* get_node(rbt_node* subt, int data);

  // Given a node, return its parent node
  // Returns null if supposed child node is not in tree
  // Also returns null if supposed child is the root node
  rbt_node* get_parent(rbt_node* child);

  // size returns the number of nodes in the subtree pointed to by subt. If the
  // tree is empty (t is NULL), it returns zero.
  int size(rbt_node* subt);

  // to_vector fills an integer vector to reflect the contents of the subtree
  // pointed to by subt. Size of the filled array will be the same as the
  // subtree's size (found with the size() function), and the order of the array
  // elements are the same that is found during an INORDER traversal of the
  // subtree.
  //
  // Note: the vector "vec" will be passed to this function as an empty vector
  // and you can add elements to it by using push_back() member function (e.g.
  // vec.push_back(4) adds 4 to the end of the vector).
  void to_vector(rbt_node* subt, vector<int>& vec);

  // Returns the root pointer.
  rbt_node* get_root();

  // Sets a given pointer as the new root pointer.
  void set_root(rbt_node** new_root);

private:
  // this double pointer always will point to the root pointer of the tree
  rbt_node** root;

  // this function finds the "grandparent" node
  // If the node or its parent is the root, returns nullptr
  rbt_node* find_gramps(rbt_node* node);

  // finds the "uncle" node
  // that is, the sibling of the parent node
  // returns nullptr if none exists
  rbt_node* find_uncle(rbt_node* node);

  rbt_node* get_sibling(rbt_node* node);

  rbt_node* get_predecessor(rbt_node* node);

  // Returns true iff a node is red (null counts as black)
  bool is_non_null_red(rbt_node* node);

  // Returns true iff a node is black (including null)
  bool is_null_or_black(rbt_node* node);

  // Returns true if both children are black (including nullptr)
  // Returns false if node has any red child
  bool both_child_black(rbt_node* node);

  // Monster helper function for removal operation
  // The purpose is to recolor and rotate as needed
  // to preserve the red-black property that every path
  // has the same number of black nodes
  void prepare_to_remove(rbt_node* node);

  // Six cases to test within the prepare_to_remove function
  bool case1(rbt_node* node);
  bool case2(rbt_node* node, rbt_node* sibling);
  bool case3(rbt_node* node, rbt_node* sibling);
  bool case4(rbt_node* node, rbt_node* sibling);
  bool case5(rbt_node* node, rbt_node* sibling);
  bool case6(rbt_node* node, rbt_node* sibling);
};

#endif // RBT_H__
