#include <stdio.h>
#include <tree_alg.h>
#include <stdlib.h>

// 现在只支持二叉树，DataType==int
void treePrint(const TreeNodePtr root, const int height) {
    TreeNodePtr *stack = malloc(sizeof(TreeNodePtr) * height);
    stack[0] = root;
    int size = 1;

    // TODO: 优化fuck
    int fuck = 1;
    do {
        for(int i = fuck; i != size; ++i) {
            putchar('\t');
        }
        fuck = 0;

        for(TreeNodePtr node = stack[--size]; node; node = node->left) {
            printf("%d\t", node->data);
            stack[size++] = node;
        }
        putchar('\n');

        do {
            TreeNodePtr node = stack[size - 1];
            if(node->right != NULL) {
                stack[size - 1] = node->right;
                break;
            }
        }while (--size);
    }while (size);

    free(stack);
}