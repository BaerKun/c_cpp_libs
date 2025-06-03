#ifndef TREE_H
#define TREE_H

#ifndef TREE_DATA_TYPE
#define TREE_DATA_TYPE int
#endif

typedef TREE_DATA_TYPE DataType;
typedef struct TreeNode_ TreeNode, *TreeNodePtr;
struct TreeNode_ {
  TreeNodePtr next;
#ifndef TREE_BINARY
  TreeNodePtr firstChild;
#else
  TreeNodePtr left, right;
#ifdef TREE_AVL
  int height, deleted;
#endif
#endif
  DataType data;
};

TreeNodePtr treeNewNode(DataType data);

void treeInsertData(TreeNodePtr *node2child, DataType data);

void treeDestroy(TreeNodePtr root);

static inline void treeInsertNode(TreeNodePtr *const node2child,
                                  const TreeNodePtr node) {
  node->next = *node2child;
  *node2child = node;
}

static inline TreeNodePtr treeUnlink(TreeNodePtr *const node2child) {
  const TreeNodePtr node = *node2child;
  *node2child = node->next;
  node->next = 0;
  return node;
}

#endif // TREE_H
