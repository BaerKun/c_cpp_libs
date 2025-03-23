#ifndef TREE_BINARY_TREE_H
#define TREE_BINARY_TREE_H

#include <tree_struct.h>

TreeNodePtr btNewNode(DataType data);

void btDestroy(TreeNodePtr root);

TreeNodePtr btUnlink(TreeNodePtr *parent2child, TreeNodePtr (*if2children)(TreeNodePtr *));

void btDeleteNode(TreeNodePtr *parent2child, TreeNodePtr (*if2children)(TreeNodePtr *));

void btInsertNode(TreeNodePtr *parent2child, TreeNodePtr node);

void btInsertData(TreeNodePtr *parent2child, DataType data);

#endif //TREE_BINARY_TREE_H
