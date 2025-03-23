#ifndef TREE_H
#define TREE_H

#include <tree_struct.h>

TreeNodePtr treeNewNode(DataType data);

void treeInsertData(TreeNodePtr *node2child, DataType data);

void treeDestroy(TreeNodePtr root);

static inline void treeInsertNode(TreeNodePtr *const node2child, const TreeNodePtr node) {
    node->next = *node2child;
    *node2child = node;
}

static inline TreeNodePtr treeUnlink(TreeNodePtr *const node2child) {
    const TreeNodePtr node = *node2child;
    *node2child = node->next;
    node->next = 0;
    return node;
}

#endif //TREE_H
