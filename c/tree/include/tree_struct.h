#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H

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

#endif //TREE_STRUCT_H
