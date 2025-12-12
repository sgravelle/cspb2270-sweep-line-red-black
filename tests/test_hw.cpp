// If you change anything in this file, your changes will be ignored
// in your homework submission.
// Chekout TEST_F functions bellow to learn what is being tested.
#include "../code/BST.h"
#include <gtest/gtest.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class test_BST : public ::testing::Test {
protected:
  // This function runs only once before any TEST_F function
  static void SetUpTestCase() {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)0;
      outgrade.close();
    }
  }

  // This function runs after all TEST_F functions have been executed
  static void TearDownTestCase() {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)(100 * total_grade / max_grade);
      outgrade.close();

      std::cout << "Total Grade is : " << (int)(100 * total_grade / max_grade)
                << std::endl;
    }
  }

  void add_points_to_grade(double points) {
    if (!::testing::Test::HasFailure()) {
      total_grade += points;
    }
  }

  // this function runs before every TEST_F function
  void SetUp() override {}

  // this function runs after ever TEST_F function
  void TearDown() override {
    std::ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)(100 * total_grade / max_grade);
      outgrade.close();
    }
  }

  static double total_grade;
  static double max_grade;
};

double test_BST::total_grade = 0;
double test_BST::max_grade = 52;

bst_node* HandBuildNode(int data) {
  bst_node* top(new bst_node);
  top->data = data;
  top->left = NULL;
  top->right = NULL;

  return top;
}

bst_node* HandBuildTree() {
  /* Hand build an unbalanced tree with 0, 1, 2, 3, 4, 5 in it. Looks
     like this:

        1
       / \
      0   4
         /  \
        3    5
       /
      2
  */
  bst_node* top = HandBuildNode(1);
  top->left = HandBuildNode(0);
  top->right = HandBuildNode(4);
  top->right->left = HandBuildNode(3);
  top->right->left->left = HandBuildNode(2);
  top->right->right = HandBuildNode(5);

  return top;
}

TEST_F(test_BST, TestConstructor) {
  BST mybst;
  // expect root to be NULL when BST object constructed
  ASSERT_FALSE(mybst.get_root());
  add_points_to_grade(2);
}

TEST_F(test_BST, TestInit) {
  BST mybst;
  bst_node* nodeptr = mybst.init_node(4);
  ASSERT_TRUE(nodeptr);
  add_points_to_grade(1);
  ASSERT_EQ(4, nodeptr->data);
  add_points_to_grade(1);
  ASSERT_FALSE(nodeptr->left);
  add_points_to_grade(1);
  ASSERT_FALSE(nodeptr->right);
  add_points_to_grade(1);
}

TEST_F(test_BST, TestInsert) {
  // hand build four nodes
  bst_node* top(new bst_node());
  top->data = 2;
  top->left = NULL;
  top->right = NULL;
  bst_node* one(new bst_node());
  one->data = 1;
  one->left = NULL;
  one->right = NULL;
  bst_node* three(new bst_node());
  three->data = 3;
  three->left = NULL;
  three->right = NULL;
  bst_node* four(new bst_node());
  four->data = 4;
  four->left = NULL;
  four->right = NULL;

  // insert them
  BST mybst;
  mybst.insert(top);
  mybst.insert(one);
  mybst.insert(three);
  mybst.insert(four);

  // expect pointer addresses to be same
  ASSERT_EQ(one, top->left);
  add_points_to_grade(2);
  ASSERT_EQ(three, top->right);
  add_points_to_grade(2);
  ASSERT_EQ(four, top->right->right);
  add_points_to_grade(2);
}

TEST_F(test_BST, TestInsertData) {

  // insert them
  BST mybst;
  mybst.insert_data(2);
  mybst.insert_data(1);
  mybst.insert_data(3);
  mybst.insert_data(4);

  // root shouldn't be null
  ASSERT_TRUE(mybst.get_root());
  add_points_to_grade(0.7);

  // check for values in the nodes
  ASSERT_EQ(2, mybst.get_root()->data);
  add_points_to_grade(0.7);
  ASSERT_EQ(1, mybst.get_root()->left->data);
  add_points_to_grade(0.7);
  ASSERT_EQ(3, mybst.get_root()->right->data);
  add_points_to_grade(0.7);
  ASSERT_EQ(4, mybst.get_root()->right->right->data);
  add_points_to_grade(0.7);

  // check for NULLs in the leafs
  ASSERT_FALSE(mybst.get_root()->left->left);
  add_points_to_grade(0.7);
  ASSERT_FALSE(mybst.get_root()->left->right);
  add_points_to_grade(0.7);
  ASSERT_FALSE(mybst.get_root()->right->left);
  add_points_to_grade(0.7);
  ASSERT_FALSE(mybst.get_root()->right->right->left);
  add_points_to_grade(0.7);
  ASSERT_FALSE(mybst.get_root()->right->right->right);
  add_points_to_grade(0.7);
}

TEST_F(test_BST, TestSize) {
  // hand build a tree of size 6
  bst_node* top = HandBuildTree();

  BST mybst;
  ASSERT_EQ(6, mybst.size(top));
  add_points_to_grade(6);
}

TEST_F(test_BST, TestContains) {
  // hand build a tree, check HandBuildTree() function in this file
  // for how the tree will look like
  bst_node* top = HandBuildTree();

  BST mybst;

  ASSERT_TRUE(mybst.contains(top, 0));
  ASSERT_TRUE(mybst.contains(top, 1));
  add_points_to_grade(0.8);
  ASSERT_TRUE(mybst.contains(top, 2));
  add_points_to_grade(0.8);
  ASSERT_TRUE(mybst.contains(top, 3));
  add_points_to_grade(0.8);
  ASSERT_TRUE(mybst.contains(top, 4));
  add_points_to_grade(0.8);
  ASSERT_TRUE(mybst.contains(top, 5));
  add_points_to_grade(0.8);

  ASSERT_FALSE(mybst.contains(top, 22));
  add_points_to_grade(0.8);
  ASSERT_FALSE(mybst.contains(top, -1));
  add_points_to_grade(0.8);
  ASSERT_FALSE(mybst.contains(top, 10));
  add_points_to_grade(0.8);
  ASSERT_FALSE(mybst.contains(top, 6));
  add_points_to_grade(0.8);
  ASSERT_FALSE(mybst.contains(top, 78));
  add_points_to_grade(0.8);
}

TEST_F(test_BST, TestGetNode) {

  // test getting a node from empty tree
  {
    BST mybst;
    bst_node* empty(NULL);
    bst_node* result = mybst.get_node(empty, 50);
    // result should be NULL
    ASSERT_FALSE(result);
    add_points_to_grade(1);
  }
  // tests getting nodes that we know are NOT there.
  {
    BST mybst;
    bst_node* top = HandBuildTree();
    ASSERT_FALSE(mybst.get_node(top, 42));
    add_points_to_grade(0.5);
    ASSERT_FALSE(mybst.get_node(top, -1));
    add_points_to_grade(0.5);
  }
  // tests getting nodes that we know are there.
  {
    BST mybst;
    bst_node* top = HandBuildTree();
    ASSERT_EQ(mybst.get_node(top, 0), top->left);
    add_points_to_grade(0.5);
    ASSERT_EQ(mybst.get_node(top, 1), top);
    add_points_to_grade(0.5);
    ASSERT_EQ(mybst.get_node(top, 2), top->right->left->left);
    add_points_to_grade(0.5);
    ASSERT_EQ(mybst.get_node(top, 3), top->right->left);
    add_points_to_grade(0.5);
    ASSERT_EQ(mybst.get_node(top, 4), top->right);
    add_points_to_grade(0.5);
    ASSERT_EQ(mybst.get_node(top, 5), top->right->right);
    add_points_to_grade(0.5);
  }
}

TEST_F(test_BST, TestRemoveNoChildren) {

  // Hand build a node with 0, 1, 2, 3, 4, 5 in it.
  bst_node* top = HandBuildTree();

  BST mybst;
  mybst.set_root(&top);

  // remove the leaf node 5
  // try removing a leaf
  mybst.remove(5);
  ASSERT_FALSE(top->right->right);
  add_points_to_grade(0.5);
}

TEST_F(test_BST, TestRemoveOneChild) {
  bst_node* top = HandBuildTree();

  BST mybst;
  mybst.set_root(&top);

  mybst.remove(3);
  ASSERT_FALSE(top->right->left->left);
  add_points_to_grade(0.5);
  ASSERT_EQ(2, top->right->left->data);
  add_points_to_grade(0.5);
}

TEST_F(test_BST, TestRemoveRootWithTwoChildren) {
  bst_node* top = HandBuildTree();
  BST mybst;
  mybst.set_root(&top);
  // Remove a node with two children (root)
  mybst.remove(1);
  ASSERT_EQ(2, mybst.get_root()->data);
  add_points_to_grade(0.25);
  ASSERT_FALSE(mybst.get_root()->right->left->left);
  add_points_to_grade(0.25);
}

TEST_F(test_BST, TestRemoveInnerWithTwoChildren) {
  bst_node* top = HandBuildTree();
  BST mybst;
  mybst.set_root(&top);
  // Remove a node with two children (non-root)
  mybst.remove(4);
  ASSERT_EQ(5, top->right->data);
  add_points_to_grade(1);
  ASSERT_EQ(1, top->data);
  add_points_to_grade(1);
  ASSERT_FALSE(top->right->right);
  add_points_to_grade(1);
}

bool VectorContains(vector<int>& vec, int value) {
  for (unsigned int i = 0; i < vec.size(); i++) {
    if (vec[i] == value) {
      return true;
    }
  }
  return false;
}

TEST_F(test_BST, TestToVector) {

  BST mybst;
  bst_node* top = HandBuildTree();

  // create a std::vector of size 6 with all values initialized to -1
  vector<int> vec;

  // ToVector function should update all vector indices with values from tree
  mybst.to_vector(top, vec);

  // check if vec was filled in
  ASSERT_EQ(6, vec.size());
  add_points_to_grade(1);

  // check if values were actually from the tree
  ASSERT_TRUE(VectorContains(vec, 0));
  add_points_to_grade(0.5);
  ASSERT_TRUE(VectorContains(vec, 1));
  add_points_to_grade(0.5);
  ASSERT_TRUE(VectorContains(vec, 2));
  add_points_to_grade(0.7);
  ASSERT_TRUE(VectorContains(vec, 3));
  add_points_to_grade(0.7);
  ASSERT_TRUE(VectorContains(vec, 4));
  add_points_to_grade(0.7);
  ASSERT_TRUE(VectorContains(vec, 5));
  add_points_to_grade(0.7);

  // check if values were from inorder walk of the tree
  ASSERT_EQ(0, vec[0]);
  add_points_to_grade(0.7);
  ASSERT_EQ(1, vec[1]);
  add_points_to_grade(0.7);
  ASSERT_EQ(2, vec[2]);
  add_points_to_grade(0.7);
  ASSERT_EQ(3, vec[3]);
  add_points_to_grade(0.7);
  ASSERT_EQ(4, vec[4]);
  add_points_to_grade(0.7);
  ASSERT_EQ(5, vec[5]);
  add_points_to_grade(0.7);
}
