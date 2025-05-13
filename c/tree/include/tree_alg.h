#ifndef TREE_ALG_H
#define TREE_ALG_H

#include "binary_search_tree.h"

typedef int DataType;
typedef int WeightType;

// 输入需要data从大到小排序
TreeNodePtr optimalBST(const DataType data[], const WeightType weight[], int number);

TreeNodePtr HuffmanCode(const DataType data[], const WeightType weight[], int number);

void treePrint(TreeNodePtr root, int height);

#endif //TREE_ALG_H
