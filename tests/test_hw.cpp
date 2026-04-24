// RBT unit tests
// Covers: constructor, init_node, get_parent, rotate_left/right,
//         bst_insert, insert/insert_data, contains, get_node,
//         size, to_vector, remove.
//
// Several helper functions below verify red-black tree invariants:
//   (1) Root is black.
//   (2) No red node has a red child ("no red-red violation").
//   (3) Every path from a node down to a NULL leaf passes through
//       the same number of black nodes ("consistent black height").
//   (4) The data satisfies the BST ordering property (left < node,
//       right >= node).

#include "../code/RBT.h"
#include <gtest/gtest.h>

#include <climits>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ===========================================================================
// Helper functions
// ===========================================================================

// Returns the black-height of the subtree rooted at `node`
// (counting black nodes on any path to NULL, not counting NULL itself).
// Returns -1 if the subtree violates the black-height invariant.
int black_height(rbt_node* node) {
  if (node == nullptr) {
    return 0;
  }
  int left_bh  = black_height(node->left);
  int right_bh = black_height(node->right);

  if (left_bh == -1 || right_bh == -1) {
    return -1;   // propagate failure upward
  }
  if (left_bh != right_bh) {
    return -1;   // black-height mismatch at this node
  }
  // Add 1 for this node only if it is black
  return left_bh + (node->is_red ? 0 : 1);
}

// Returns true if no red node in the subtree has a red child.
bool no_red_red(rbt_node* node) {
  if (node == nullptr) {
    return true;
  }
  if (node->is_red) {
    if (node->left  != nullptr && node->left->is_red)  return false;
    if (node->right != nullptr && node->right->is_red) return false;
  }
  return no_red_red(node->left) && no_red_red(node->right);
}

// Returns true if the subtree rooted at `node` obeys the BST ordering
// property: all values in the left subtree are strictly less than node->data,
// all values in the right subtree are >= node->data.
// `lo` and `hi` are the inclusive bounds for values in this subtree.
bool valid_bst(rbt_node* node, int lo = INT_MIN, int hi = INT_MAX) {
  if (node == nullptr) {
    return true;
  }
  if (node->data < lo || node->data > hi) {
    return false;
  }
  // left subtree: all values must be < node->data (i.e. hi = node->data - 1)
  // right subtree: all values must be >= node->data (i.e. lo = node->data)
  return valid_bst(node->left,  lo,            node->data - 1) &&
         valid_bst(node->right, node->data,    hi);
}

// Bundles the three structural checks above into one call.
// `root` is the root pointer of the whole tree (to check root colour).
bool valid_rbt(rbt_node* root) {
  if (root == nullptr) {
    return true;   // empty tree is trivially valid
  }
  if (root->is_red) {
    return false;  // root must be black
  }
  if (!no_red_red(root)) {
    return false;
  }
  if (black_height(root) == -1) {
    return false;
  }
  if (!valid_bst(root)) {
    return false;
  }
  return true;
}

// Hand-build a tiny RBT node (both children NULL, coloured black by default).
rbt_node* make_node(int data, bool is_red = false) {
  rbt_node* n = new rbt_node;
  n->data   = data;
  n->is_red = is_red;
  n->left   = nullptr;
  n->right  = nullptr;
  return n;
}

// ===========================================================================
// Test fixture
// ===========================================================================

class test_RBT : public ::testing::Test {
protected:
  static void SetUpTestCase()    {}
  static void TearDownTestCase() {}
  void SetUp()    override {}
  void TearDown() override {}
};

// ===========================================================================
// Constructor
// ===========================================================================

TEST_F(test_RBT, TestConstructor) {
  RBT myrbt;
  // A newly constructed tree should have no root.
  ASSERT_FALSE(myrbt.get_root());
}

// ===========================================================================
// init_node
// ===========================================================================

TEST_F(test_RBT, TestInitNode) {
  RBT myrbt;
  rbt_node* n = myrbt.init_node(7);

  ASSERT_TRUE(n);
  ASSERT_EQ(7,      n->data);
  ASSERT_FALSE(n->is_red);   // init_node must colour the node BLACK
  ASSERT_FALSE(n->left);
  ASSERT_FALSE(n->right);

  delete n;
}

// ===========================================================================
// get_parent
// ===========================================================================

TEST_F(test_RBT, TestGetParentRoot) {
  // The root has no parent, so get_parent should return NULL.
  RBT myrbt;
  myrbt.insert_data(10);
  ASSERT_FALSE(myrbt.get_parent(myrbt.get_root()));
}

TEST_F(test_RBT, TestGetParentNotInTree) {
  // A node that was not inserted returns NULL.
  RBT myrbt;
  myrbt.insert_data(10);
  myrbt.insert_data(5);

  rbt_node* outsider = make_node(99);
  ASSERT_FALSE(myrbt.get_parent(outsider));
  delete outsider;
}

TEST_F(test_RBT, TestGetParentBasic) {
  // After inserting a few values, the parent of a known child should be
  // its direct ancestor.
  RBT myrbt;
  myrbt.insert_data(10);
  myrbt.insert_data(5);
  myrbt.insert_data(15);

  rbt_node* root = myrbt.get_root();
  // Whatever the root value is (could be 10 after rebalancing), the
  // children's parents must be the root.
  if (root->left) {
    ASSERT_EQ(root, myrbt.get_parent(root->left));
  }
  if (root->right) {
    ASSERT_EQ(root, myrbt.get_parent(root->right));
  }
}

// ===========================================================================
// rotate_left / rotate_right
// ===========================================================================

// For an in-place rotation on a node pointer, the canonical trick is to
// swap data rather than re-link the parent.  The tests below check the
// observable tree structure after a rotation.

TEST_F(test_RBT, TestRotateLeft) {
  // Build a small subtree:
  //     2
  //    / \
  //   1   3
  //
  // After rotate_left at the root (value 2):
  //     3
  //    /
  //   2
  //  /
  // 1
  rbt_node* top   = make_node(2);
  rbt_node* left  = make_node(1);
  rbt_node* right = make_node(3);
  top->left  = left;
  top->right = right;

  RBT myrbt;
  myrbt.rotate_left(top);

  // The physical node `top` now contains what was in the right child.
  ASSERT_EQ(3, top->data);
  ASSERT_TRUE(top->left);
  ASSERT_EQ(2, top->left->data);
  ASSERT_EQ(1, top->left->left->data);
  ASSERT_FALSE(top->right);         // 3 had no right child
  ASSERT_FALSE(top->left->right);   // 3 had no left child, becomes 2's right
}

TEST_F(test_RBT, TestRotateRight) {
  // Build a small subtree:
  //     2
  //    / \
  //   1   3
  //
  // After rotate_right at the root (value 2):
  //   1
  //    \
  //     2
  //      \
  //       3
  rbt_node* top   = make_node(2);
  rbt_node* left  = make_node(1);
  rbt_node* right = make_node(3);
  top->left  = left;
  top->right = right;

  RBT myrbt;
  myrbt.rotate_right(top);

  ASSERT_EQ(1, top->data);
  ASSERT_TRUE(top->right);
  ASSERT_EQ(2, top->right->data);
  ASSERT_EQ(3, top->right->right->data);
  ASSERT_FALSE(top->left);          // 1 had no left child
  ASSERT_FALSE(top->right->left);   // 1 had no right child, becomes 2's left
}

TEST_F(test_RBT, TestRotateLeftPreservesSubtrees) {
  // Build:
  //      4
  //     / \
  //    2   6
  //   / \ / \
  //  1  3 5  7
  //
  // rotate_left at 4.  Expected:
  //      6
  //     / \
  //    4   7
  //   / \
  //  2   5
  // / \
  //1   3
  rbt_node* n4 = make_node(4);
  rbt_node* n2 = make_node(2);
  rbt_node* n6 = make_node(6);
  rbt_node* n1 = make_node(1);
  rbt_node* n3 = make_node(3);
  rbt_node* n5 = make_node(5);
  rbt_node* n7 = make_node(7);
  n4->left  = n2;  n4->right = n6;
  n2->left  = n1;  n2->right = n3;
  n6->left  = n5;  n6->right = n7;

  RBT myrbt;
  myrbt.rotate_left(n4);

  ASSERT_EQ(6, n4->data);
  ASSERT_EQ(7, n4->right->data);
  ASSERT_EQ(4, n4->left->data);
  ASSERT_EQ(5, n4->left->right->data);  // 6's old left child becomes 4's right
  ASSERT_EQ(2, n4->left->left->data);
  ASSERT_EQ(1, n4->left->left->left->data);
  ASSERT_EQ(3, n4->left->left->right->data);
}

TEST_F(test_RBT, TestRotateRightPreservesSubtrees) {
  // Mirror of the above: rotate_right at 4.  Expected:
  //      2
  //     / \
  //    1   4
  //       / \
  //      3   6
  //         / \
  //        5   7
  rbt_node* n4 = make_node(4);
  rbt_node* n2 = make_node(2);
  rbt_node* n6 = make_node(6);
  rbt_node* n1 = make_node(1);
  rbt_node* n3 = make_node(3);
  rbt_node* n5 = make_node(5);
  rbt_node* n7 = make_node(7);
  n4->left  = n2;  n4->right = n6;
  n2->left  = n1;  n2->right = n3;
  n6->left  = n5;  n6->right = n7;

  RBT myrbt;
  myrbt.rotate_right(n4);

  ASSERT_EQ(2, n4->data);
  ASSERT_EQ(1, n4->left->data);
  ASSERT_EQ(4, n4->right->data);
  ASSERT_EQ(3, n4->right->left->data);  // 2's old right child becomes 4's left
  ASSERT_EQ(6, n4->right->right->data);
  ASSERT_EQ(5, n4->right->right->left->data);
  ASSERT_EQ(7, n4->right->right->right->data);
}

// ===========================================================================
// bst_insert (raw BST insert, NO rebalancing)
// ===========================================================================

TEST_F(test_RBT, TestBstInsert) {
  // bst_insert places nodes exactly like a plain BST.
  // Inserting 2, 1, 3, 4 should produce:
  //   2
  //  / \
  // 1   3
  //      \
  //       4
  RBT myrbt;
  rbt_node* n2 = myrbt.init_node(2);
  rbt_node* n1 = myrbt.init_node(1);
  rbt_node* n3 = myrbt.init_node(3);
  rbt_node* n4 = myrbt.init_node(4);

  myrbt.bst_insert(n2);
  myrbt.bst_insert(n1);
  myrbt.bst_insert(n3);
  myrbt.bst_insert(n4);

  ASSERT_EQ(n2, myrbt.get_root());
  ASSERT_EQ(n1, myrbt.get_root()->left);
  ASSERT_EQ(n3, myrbt.get_root()->right);
  ASSERT_EQ(n4, myrbt.get_root()->right->right);
  ASSERT_FALSE(myrbt.get_root()->left->left);
  ASSERT_FALSE(myrbt.get_root()->left->right);
  ASSERT_FALSE(myrbt.get_root()->right->left);
}

TEST_F(test_RBT, TestBstInsertPreservesOrdering) {
  // Whatever shape bst_insert produces, the BST ordering property must hold.
  RBT myrbt;
  int vals[] = {5, 2, 8, 1, 4, 7, 9};
  for (int v : vals) {
    myrbt.bst_insert(myrbt.init_node(v));
  }
  ASSERT_TRUE(valid_bst(myrbt.get_root()));
}

// ===========================================================================
// insert / insert_data  (full RBT insert)
// ===========================================================================

TEST_F(test_RBT, TestInsertSingleNode) {
  RBT myrbt;
  myrbt.insert_data(10);

  rbt_node* root = myrbt.get_root();
  ASSERT_TRUE(root);
  ASSERT_EQ(10, root->data);
  // Root must be black after any insert.
  ASSERT_FALSE(root->is_red);
}

TEST_F(test_RBT, TestInsertRootIsAlwaysBlack) {
  // After every insert, the root must remain black.
  RBT myrbt;
  int vals[] = {1, 2, 3, 4, 5, 6, 7};
  for (int v : vals) {
    myrbt.insert_data(v);
    ASSERT_FALSE(myrbt.get_root()->is_red)
        << "Root became red after inserting " << v;
  }
}

TEST_F(test_RBT, TestInsertNoRedRedViolation) {
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20, 1, 4, 6, 8};
  for (int v : vals) {
    myrbt.insert_data(v);
    ASSERT_TRUE(no_red_red(myrbt.get_root()))
        << "Red-red violation after inserting " << v;
  }
}

TEST_F(test_RBT, TestInsertBlackHeightConsistent) {
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20, 1, 4, 6, 8};
  for (int v : vals) {
    myrbt.insert_data(v);
    ASSERT_NE(-1, black_height(myrbt.get_root()))
        << "Black-height inconsistency after inserting " << v;
  }
}

TEST_F(test_RBT, TestInsertBstOrderingPreserved) {
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20, 1, 4, 6, 8};
  for (int v : vals) {
    myrbt.insert_data(v);
    ASSERT_TRUE(valid_bst(myrbt.get_root()))
        << "BST ordering violated after inserting " << v;
  }
}

TEST_F(test_RBT, TestInsertFullValidation) {
  // Aggregate check using valid_rbt helper.
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20, 1, 4, 6, 8};
  for (int v : vals) {
    myrbt.insert_data(v);
    ASSERT_TRUE(valid_rbt(myrbt.get_root()))
        << "RBT invariant violated after inserting " << v;
  }
}

TEST_F(test_RBT, TestInsertAscendingOrder) {
  // Worst case for a plain BST; RBT should stay balanced.
  RBT myrbt;
  for (int i = 1; i <= 15; i++) {
    myrbt.insert_data(i);
  }
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
  ASSERT_EQ(15, myrbt.size(myrbt.get_root()));
}

TEST_F(test_RBT, TestInsertDescendingOrder) {
  RBT myrbt;
  for (int i = 15; i >= 1; i--) {
    myrbt.insert_data(i);
  }
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
  ASSERT_EQ(15, myrbt.size(myrbt.get_root()));
}

TEST_F(test_RBT, TestInsertContainsAllValues) {
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20};
  for (int v : vals) {
    myrbt.insert_data(v);
  }
  for (int v : vals) {
    ASSERT_TRUE(myrbt.contains(myrbt.get_root(), v))
        << "Missing value " << v << " after insert";
  }
}

TEST_F(test_RBT, TestInsertNodePointer) {
  // insert() (pointer version) should behave the same as insert_data().
  RBT myrbt;
  rbt_node* n = myrbt.init_node(42);
  myrbt.insert(n);
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
  ASSERT_TRUE(myrbt.contains(myrbt.get_root(), 42));
}

// ===========================================================================
// contains
// ===========================================================================

TEST_F(test_RBT, TestContainsBasic) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) {
    myrbt.insert_data(v);
  }

  for (int v : vals) {
    ASSERT_TRUE(myrbt.contains(myrbt.get_root(), v));
  }

  ASSERT_FALSE(myrbt.contains(myrbt.get_root(),  0));
  ASSERT_FALSE(myrbt.contains(myrbt.get_root(),  8));
  ASSERT_FALSE(myrbt.contains(myrbt.get_root(), -1));
  ASSERT_FALSE(myrbt.contains(myrbt.get_root(), 42));
}

TEST_F(test_RBT, TestContainsEmptyTree) {
  RBT myrbt;
  ASSERT_FALSE(myrbt.contains(nullptr, 5));
}

// ===========================================================================
// get_node
// ===========================================================================

TEST_F(test_RBT, TestGetNodeEmptyTree) {
  RBT myrbt;
  ASSERT_FALSE(myrbt.get_node(nullptr, 5));
}

TEST_F(test_RBT, TestGetNodeNotPresent) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  ASSERT_FALSE(myrbt.get_node(myrbt.get_root(), 99));
  ASSERT_FALSE(myrbt.get_node(myrbt.get_root(), -5));
}

TEST_F(test_RBT, TestGetNodeReturnsCorrectPointer) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  // For every value we inserted, get_node should return a non-null pointer
  // whose data field equals the requested value.
  for (int v : vals) {
    rbt_node* found = myrbt.get_node(myrbt.get_root(), v);
    ASSERT_TRUE(found)       << "get_node returned NULL for value " << v;
    ASSERT_EQ(v, found->data) << "get_node returned wrong node for value " << v;
  }
}

// ===========================================================================
// size
// ===========================================================================

TEST_F(test_RBT, TestSizeEmpty) {
  RBT myrbt;
  ASSERT_EQ(0, myrbt.size(nullptr));
}

TEST_F(test_RBT, TestSizeAfterInserts) {
  RBT myrbt;
  for (int i = 0; i < 7; i++) {
    myrbt.insert_data(i);
    ASSERT_EQ(i + 1, myrbt.size(myrbt.get_root()));
  }
}

// ===========================================================================
// to_vector
// ===========================================================================

TEST_F(test_RBT, TestToVectorEmpty) {
  RBT myrbt;
  vector<int> vec;
  myrbt.to_vector(nullptr, vec);
  ASSERT_EQ(0u, vec.size());
}

TEST_F(test_RBT, TestToVectorInorder) {
  // to_vector must produce an in-order (sorted ascending) traversal.
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  vector<int> vec;
  myrbt.to_vector(myrbt.get_root(), vec);

  ASSERT_EQ(7u, vec.size());
  for (int i = 0; i < (int)vec.size(); i++) {
    ASSERT_EQ(i + 1, vec[i]) << "In-order mismatch at index " << i;
  }
}

TEST_F(test_RBT, TestToVectorSizeMatchesTreeSize) {
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20};
  for (int v : vals) myrbt.insert_data(v);

  vector<int> vec;
  myrbt.to_vector(myrbt.get_root(), vec);

  ASSERT_EQ((unsigned)myrbt.size(myrbt.get_root()), vec.size());
}

// ===========================================================================
// remove
// ===========================================================================

TEST_F(test_RBT, TestRemoveLeaf) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  myrbt.remove(1);

  ASSERT_FALSE(myrbt.contains(myrbt.get_root(), 1));
  ASSERT_EQ(6, myrbt.size(myrbt.get_root()));
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
}

TEST_F(test_RBT, TestRemoveInnerNode) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  myrbt.remove(2);

  ASSERT_FALSE(myrbt.contains(myrbt.get_root(), 2));
  ASSERT_EQ(6, myrbt.size(myrbt.get_root()));
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
}

TEST_F(test_RBT, TestRemoveRoot) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  int old_root_data = myrbt.get_root()->data;
  myrbt.remove(old_root_data);

  ASSERT_FALSE(myrbt.contains(myrbt.get_root(), old_root_data));
  ASSERT_EQ(6, myrbt.size(myrbt.get_root()));
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
}

TEST_F(test_RBT, TestRemoveNonExistent) {
  // Removing a value that isn't in the tree should have no effect.
  RBT myrbt;
  int vals[] = {4, 2, 6};
  for (int v : vals) myrbt.insert_data(v);

  myrbt.remove(99);

  ASSERT_EQ(3, myrbt.size(myrbt.get_root()));
  ASSERT_TRUE(valid_rbt(myrbt.get_root()));
}

TEST_F(test_RBT, TestRemoveAllValues) {
  RBT myrbt;
  int vals[] = {4, 2, 6, 1, 3, 5, 7};
  for (int v : vals) myrbt.insert_data(v);

  for (int v : vals) {
    myrbt.remove(v);
    // After each removal the tree should still be a valid RBT
    // (or empty, which is also valid).
    if (myrbt.get_root() != nullptr) {
      ASSERT_TRUE(valid_rbt(myrbt.get_root()))
          << "RBT invariant violated after removing " << v;
    }
  }
  ASSERT_FALSE(myrbt.get_root());
}

TEST_F(test_RBT, TestRemovePreservesContents) {
  // After removing one value, all other inserted values should still be present.
  RBT myrbt;
  int vals[] = {10, 5, 15, 3, 7, 12, 20};
  for (int v : vals) myrbt.insert_data(v);

  myrbt.remove(5);

  ASSERT_FALSE(myrbt.contains(myrbt.get_root(), 5));
  int remaining[] = {10, 15, 3, 7, 12, 20};
  for (int v : remaining) {
    ASSERT_TRUE(myrbt.contains(myrbt.get_root(), v))
        << "Value " << v << " disappeared after removing 5";
  }
}

TEST_F(test_RBT, TestRemoveMaintainsInorder) {
  // After removals, to_vector should still produce a sorted sequence.
  RBT myrbt;
  for (int i = 1; i <= 10; i++) myrbt.insert_data(i);

  myrbt.remove(3);
  myrbt.remove(7);
  myrbt.remove(1);

  vector<int> vec;
  myrbt.to_vector(myrbt.get_root(), vec);

  for (int i = 1; i < (int)vec.size(); i++) {
    ASSERT_LT(vec[i - 1], vec[i])
        << "Out-of-order element at index " << i << " after removals";
  }
}