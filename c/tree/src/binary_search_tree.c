#include <binary_search_tree.h>
#include <binary_tree.h>
#include <stdio.h>
#include <stdlib.h>

TreeNodePtr *bstFind(TreeNodePtr *const root, const DataType data) {
    TreeNodePtr *parent2child = root;

    for (TreeNodePtr node = *parent2child; node != NULL; node = *parent2child) {
        if (LESS(data, node->data))
            parent2child = &node->left;
        else if (GREATER(data, node->data))
            parent2child = &node->right;
        else
            break;
    }

    return parent2child;
}

TreeNodePtr *bstFindMax(TreeNodePtr *const root) {
    TreeNodePtr *parent2child = root;
    TreeNodePtr child = *parent2child;
    if (child != NULL) {
        while (child->right != NULL) {
            parent2child = &child->right;
            child = *parent2child;
        }
    }
    return parent2child;
}

TreeNodePtr bstUnlinkMax(TreeNodePtr *const root) {
    TreeNodePtr *node2max = bstFindMax(root);
    return btUnlink(node2max, NULL);
}

void bstInsertNode(TreeNodePtr *const root, const TreeNodePtr node) {
    TreeNodePtr *parent2child = bstFind(root, node->data);
    btInsertNode(parent2child, node);
}

void bstInsertData(TreeNodePtr *const root, const DataType data) {
    TreeNodePtr *parent2child = bstFind(root, data);
    btInsertData(parent2child, data);
}

static TreeNodePtr bstUnlinkRightMin(const TreeNodePtr node) {
    TreeNodePtr *parent2child = &node->right;
    TreeNodePtr child = *parent2child;
    while (child->left != NULL) {
        parent2child = &child->left;
        child = *parent2child;
    }

    return btUnlink(parent2child, NULL);
}

static TreeNodePtr if2childrenCallback(TreeNodePtr *const parent2child) {
    const TreeNodePtr node = *parent2child;

    const TreeNodePtr replace = bstUnlinkRightMin(node);
    replace->left = node->left;
    replace->right = node->right;
    replace->next = NULL;
    *parent2child = replace;
    node->left = node->right = NULL;

    return node;
}

TreeNodePtr bstUnlink(TreeNodePtr *const parent2child) {
    return btUnlink(parent2child, if2childrenCallback);
}

TreeNodePtr bstUnlinkWithData(TreeNodePtr *const root, const DataType data) {
    TreeNodePtr *const parent2child = bstFind(root, data);
    return bstUnlink(parent2child);
}

TreeNodePtr buildBST(const DataType data[], const int len, const TreeNodePtr buffer) {
    TreeNodePtr tree = NULL;

    if (buffer == NULL) {
        for (int i = 0; i < len; i++)
            bstInsertData(&tree, data[i]);
    } else {
        for (int i = 0; i < len; i++) {
            buffer[i].data = data[i];
            bstInsertNode(&tree, buffer + i);
        }
    }

    return tree;
}
