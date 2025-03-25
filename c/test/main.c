#include <tree_alg.h>
#include <stdio.h>
#include <link_like.h>

int main() {
    int data[] = {1, 2, 3, 4, 5, 6, 7};
    int weight[] = {1, 2, 3, 4, 5, 6, 7};
    TreeNodePtr root = optimalBST(data, weight, 7);
    printf("%d", root->data);
    treeDestroy(root);
    return 0;
}