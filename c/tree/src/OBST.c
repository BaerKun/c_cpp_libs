#include <stdlib.h>
#include <tree_alg.h>

typedef struct {
  int left, right;
} LeftRight;

#define QUEUE_ELEM_TYPE LeftRight
#include "queue.h"

typedef struct {
  WeightType sub; // left~right 内的 OBST 的 sum(d*w)
  WeightType sum; // sum(w[i]) left <= i <= right
  int root;       // left!right 内的 OBST 的 root
} DPData;

/*
 * 最优二叉搜索树
 * minimize(sum(deep[i] * weight[i]))
 */

// 输入data应该升序排列，以满足搜索二叉树的性质
TreeNodePtr optimalBST(const DataType data[], const WeightType weight[],
                       const int number) {
  int left, root, right;
  DPData **dp = malloc(number * sizeof(void *) +
                       number * (number + 1) / 2 * sizeof(DPData));
  DPData *dpBuff = (DPData *)(dp + number);

  /*
   *          00  01  02 ...
   *      ->  11  12  13 ...
   *  ->  -   22  23  24 ...
   *  -   -   ...
   */
  for (int i = 0, j = 0, k = number; i != number; ++i) {
    dpBuff[j].sub = weight[i];
    dpBuff[j].sum = weight[i];
    dpBuff[j].root = i;
    dp[i] = dpBuff + j - i;
    j += k--;
  }

  for (int width = 1; width < number; width++) {
    for (left = 0; (right = width + left) < number; left++) {
      dp[left][right].sum = weight[left] + dp[left + 1][right].sum;

      WeightType min; // min(left-sub-OBST + right-sub-OBST)
      if (dp[left + 1][right].sub < dp[left][right - 1].sub) {
        min = dp[left + 1][right].sub;
        dp[left][right].root = left;
      } else {
        min = dp[left][right - 1].sub;
        dp[left][right].root = right;
      }

      for (root = left + 1; root != right; ++root) {
        if (dp[left][root - 1].sub + dp[root + 1][right].sub < min) {
          min = dp[left][root - 1].sub + dp[root + 1][right].sub;
          dp[left][right].root = root;
        }
      }
      dp[left][right].sub = min + dp[left][right].sum; // 递推公式
    }
  }

  TreeNodePtr *nodes = malloc(number * sizeof(TreeNodePtr));
  for (int i = 0; i != number; ++i) nodes[i] = malloc(sizeof(TreeNode));
  const TreeNodePtr tree = nodes[dp[0][number - 1].root];

  // N个节点的二叉树最多有(N+1)/2个树叶
  Queue queue;
  queueInit(&queue, (number + 1) / 2);
  enqueue(&queue, (LeftRight){0, number - 1});

  while (queueEmpty(&queue)) {
    const LeftRight *lr = queueFront(&queue);
    left = lr->left;
    right = lr->right;
    dequeue(&queue);

    root = dp[left][right].root;
    const TreeNodePtr node = nodes[root];
    node->data = data[root];
    node->next = NULL;

    if (root != left) {
      enqueue(&queue, (LeftRight){left, root - 1});
      node->left = nodes[dp[left][root - 1].root];
    } else
      node->left = NULL;

    if (root != right) {
      enqueue(&queue, (LeftRight){root + 1, right});
      node->right = nodes[dp[root + 1][right].root];
    } else
      node->right = NULL;
  }

  free(dp);
  free(nodes);
  queueFreeData(&queue);
  return tree;
}
