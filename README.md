# Binary Search Trees

This homework is about a plain, not-balanced Binary Search Tree data structure. It
provides methods to insert and remove integers, test if the tree contains a
given value, and retrieve the size and sort order of the data it contains.

A binary search tree partitions a search space into two parts: smaller values to the left, equal or larger values to the right. (This rightward bias is arbitrary, and many other tree implementations have a leftward bias with equal values following the left path).

Because a binary tree partitions the search space in half when you follow a link to the next layer, it makes it substantially faster to find data. In fact we can mathematically analyze just how much faster it is. We'll cover computational complexity soon - but as a teaser, this "cut things in half" process makes the search process roughly logarithmic with respect to the input size. Compare this to the linear complexity with linked lists - to find a value in a linked list, you might have to traverse all `n` items. But with a binary search tree, that gets cut to `log n`.

(Now, since this plain binary search tree is not balanced, that claim isn't actually guaranteed. We'll encounter balanced trees in the near term future.)

## Double Pointer for the root

You'll notice that the root node is defined as a double pointer:

```c++
bst_node** root;
```

To initialize this, there is some suggested code in the cpp file:

```c++
BST::BST() {
  root = new bst_node*;
  *root = NULL;
}
```

This is not the only way to do this. This code is provided for you as an example
of how to initialize the root value. It also shows how you can assign into the
memory pointed to by root. And bonus! It also lets the `TestConstructor` unit
test pass. Points!

If the root node was a single pointer, what would happen if you needed to change
the root node of the tree? You'd need to have a special `set_root` method. This
is a reasonable way to do it, but not how we're doing it here.

With a double pointer, you can just assign into the `*root` reference like:

```c++
*root = new_root; // where new_root has type bst_node*
```

Back to the constructor implementation listed above: If your code uses that approach, you can rely on `root` as always being non-null, and `*root` as always pointing to the `bst_node*` at the top of the tree. If the tree is empty, then `*root == NULL`.