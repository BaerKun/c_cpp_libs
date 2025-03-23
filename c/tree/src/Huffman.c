#include <Huffman.h>

typedef struct HeapElement {
    TreeNodePtr node;
    int value;
} HeapElement;

#define HEAP_ELEMENT_TYPE HeapElement
#define HEAP_LESS_THAN(a, b) (a.value < b.value)
#include "heap.h"

TreeNodePtr HuffmanCode(const DataType *data, const int *cost, const int number) {
    const HeapPtr heap = newHeap(number);
    TreeNodePtr root = NULL;

    for (int i = 0, j = 1; i < number; i = j++) {
        const TreeNodePtr node = btNewNode(data[i]);
        heap->prev[j] = (HeapElement) {node, cost[i]};
    }

    buildHeap(heap, number);

    while (heap->size > 1) {
        const HeapElement minCost = heapDeleteMin(heap);
        root = btNewNode(NO_DATA);

        root->left = minCost.node;
        root->right = heapDeleteMin(heap).node;

        heap->prev[1].value += minCost.value;
        heap->prev[1].node = root;
        heapPercolateDown(heap->prev, 1, heap->size);
    }

    heapDestroy(heap);
    return root;
}