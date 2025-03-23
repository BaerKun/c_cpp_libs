#ifndef TREE_BINARY_SEARCH_TREE_H
#define TREE_BINARY_SEARCH_TREE_H

#include <tree_struct.h>

// 输出 x < y
#define GREATER(x, y) (x > y)
#define LESS(x, y) (x < y)
#define EQUAL(x, y) (x == y)

TreeNodePtr *bstFindMax(TreeNodePtr *root);

TreeNodePtr bstUnlinkMax(TreeNodePtr *root);

TreeNodePtr *bstFind(TreeNodePtr *root, DataType data);

void bstInsertNode(TreeNodePtr *root, TreeNodePtr node);

void bstInsertData(TreeNodePtr *root, DataType data);

TreeNodePtr bstUnlink(TreeNodePtr *parent2child);

TreeNodePtr bstUnlinkWithData(TreeNodePtr *root, DataType data);

TreeNodePtr buildBST(const DataType data[], int len, TreeNodePtr buffer);

#endif //TREE_BINARY_SEARCH_TREE_H
