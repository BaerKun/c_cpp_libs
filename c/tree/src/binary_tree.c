#include <binary_tree.h>
#include <tree.h>
#include <stdlib.h>

TreeNodePtr btNewNode(DataType data){
    return treeNewNode(data);
}

void btDestroy(TreeNodePtr root){
    treeDestroy(root);
}

TreeNodePtr btUnlink(TreeNodePtr *const parent2child, TreeNodePtr (*if2children)(TreeNodePtr *)) {
    const TreeNodePtr node = *parent2child;

    if (node == NULL)
        return NULL;

    if (node->next != NULL) {
        *parent2child = node->next;
        (*parent2child)->left = node->left;
        (*parent2child)->right = node->right;
        node->next = node->left = node->right = NULL;
    } else if (node->left == NULL) {
        *parent2child = node->right;
        node->right = NULL;
    } else if (node->right == NULL) {
        *parent2child = node->left;
        node->left = NULL;
    } else if (if2children == NULL)
        return NULL;
    else
        return if2children(parent2child);

    return node;
}

void btDeleteNode(TreeNodePtr *const parent2child, TreeNodePtr (*if2children)(TreeNodePtr *)) {
    const TreeNodePtr node = btUnlink(parent2child, if2children);
    if (node != NULL)
        free(node);
}

void btInsertNode(TreeNodePtr *const parent2child, const TreeNodePtr node) {
    const TreeNodePtr head = *parent2child;

    if (head == NULL) {
        *parent2child = node;
    } else {
        node->left = head->left;
        node->right = head->right;
        treeInsertNode(&head->next, node);
    }
}

void btInsertData(TreeNodePtr *const parent2child, const DataType data) {
    const TreeNodePtr node = btNewNode(data);
    btInsertNode(parent2child, node);
}
