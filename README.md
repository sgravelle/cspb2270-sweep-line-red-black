# Red-Black Trees

This code implements the red-black tree data structure. A red-black tree is a balanced binary tree, meaning that it has minimum depth. Therefore searching time is optimized, taking O(log(n)) time where n is the number of nodes.

This is accomplished by coloring all nodes red or black, and maintaining the invariants that:

* The root is black

* Null children are black

* No red node can have a red child

* All paths from a node to any null leaf descendant node must have the same number of black nodes.

My implementation takes our binary search tree from class as a starting point, and adds a `bool` to the node struct which is true for red nodes and false for black nodes. Whenever insertion or removal is done, the tree must be rebalanced to preserve the coloring, which is accomplished by a number of helper functions.