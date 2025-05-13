#include <tree_alg.h>

typedef struct HeapData_ {
    TreeNodePtr node;
    WeightType weight;
} HeapData;

#define HEAP_DATA_TYPE HeapData
#define HEAP_LESS_THAN(a, b) (a.weight < b.weight)
#include "heap.h"

#define NO_DATA (-1)

/*
 * minimize(deep[i] * weight[i])，且数据都在树叶
 */
TreeNodePtr HuffmanCode(const DataType data[], const WeightType weight[], const int number) {
    if (number == 1)
        return treeNewNode(*data);

    Heap heap;
    HeapData *heapData = malloc(sizeof(HeapData) * number);
    for (int i = 0; i < number; ++i) {
        const TreeNodePtr node = treeNewNode(data[i]);
        heapData[i] = (HeapData) {node, weight[i]};
    }
    buildHeap(&heap, heapData, number);

    TreeNodePtr root = NULL;

    while (heap.size > 1) {
        // 弹出heap的top2，作为root的left/right，然后将root重新入堆，root的weight为left、right之和
        // 这里合并了弹出right和插入root
        const HeapData minCost = *heapTop(&heap);
        heapPop(&heap);

        root = treeNewNode(NO_DATA);
        root->left = minCost.node;
        root->right = heapTop(&heap)->node;

        heap.data[0].weight += minCost.weight;
        heap.data[0].node = root;
        heapPercolateDown(&heap, 1, heap.size);
    }

    free(heapData);
    return root;
}