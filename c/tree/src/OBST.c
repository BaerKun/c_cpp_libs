#include "OBST.h"
#include "queue.h"
#include <stdlib.h>

#define MAX_COUNT_INPUT_SIZE 64

typedef struct {
    WeightType dp;
    WeightType sum;
    int root;
} DpWeight;

/*
 * 最优二叉搜索树
 * minimize(sum(deep[i] * weight[i]))
 */
TreeNodePtr optimalBST(const DataType data[], const WeightType weight[], const int number, void **buffer) {
    int left, root, right;
    WeightType minTreeWeight;
    DpWeight *dpWeight = malloc(number * (number + 1) / 2 * sizeof(DpWeight));

    for (int i = 0; i < number; ++i) {
        dpWeight[i].dp = weight[i];
        dpWeight[i].sum = weight[i];
        dpWeight[i].root = i;
    }

    // TODO: 想一种更高效的内存储存方法
    // treeWeight[right][left] = sum(weight[i]) left <= i <= right
    for (int width = 1; width < number; width++) {
        for (left = 0; (right = width + left) < number; left++) {
            int i = 0;

            treeWeight[right][left] = treeWeight[left][left] + treeWeight[right][left + 1];

            if (treeWeight[left + 1][right] < treeWeight[left][right - 1]) {
                minTreeWeight = treeWeight[left + 1][right];
                treeRoot[left][right] = left;
            } else {
                minTreeWeight = treeWeight[left][right - 1];
                treeRoot[left][right] = right;
            }

            for (root = left + 1; root < right; root++) {
                if (treeWeight[left][root - 1] + treeWeight[root + 1][right] < minTreeWeight) {
                    minTreeWeight = treeWeight[left][root - 1] + treeWeight[root + 1][right];
                    treeRoot[left][right] = root;
                }
            }

            treeWeight[left][right] = minTreeWeight + treeWeight[right][left];
        }
    }

    free(treeWeight);

    const QueuePtr pLeftQueue = newQueue(number);
    const QueuePtr pRightQueue = newQueue(number);
    const TreeNodePtr _buffer = malloc(number * sizeof(TreeNode));

    enqueue(pLeftQueue, 0);
    enqueue(pRightQueue, number - 1);

    const TreeNodePtr tree = _buffer + treeRoot[0][number - 1];

    while (pLeftQueue->front != pLeftQueue->rear) {
        left = dequeue(pLeftQueue);
        right = dequeue(pRightQueue);
        root = treeRoot[left][right];
        const TreeNodePtr rootNode = _buffer + root;

        rootNode->data = data[root];
        rootNode->next = NULL;

        if (root > left) {
            enqueue(pLeftQueue, left);
            enqueue(pRightQueue, root - 1);
            left = treeRoot[left][root - 1];
            rootNode->left = _buffer + left;
        } else
            rootNode->left = NULL;

        if (root < right) {
            enqueue(pLeftQueue, root + 1);
            enqueue(pRightQueue, right);
            right = treeRoot[root + 1][right];
            rootNode->right = _buffer + right;
        } else
            rootNode->right = NULL;
    }

    free(treeRoot);
    queue_destroy(pLeftQueue);
    queue_destroy(pRightQueue);

    *buffer = _buffer;
    return tree;
}
