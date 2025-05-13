#include "only_edge/min_spanning_tree.h"
#include "disjoint_set.h"

#define HEAP_DATA_TYPE EdgePtr
#define HEAP_LESS_THAN(a, b) ((a)->data.weight < (b)->data.weight)
#include "heap.h"

#include <stdio.h>

void KruskalMinSpanningTree(const GraphPtr graph, EdgeId outputArray[]) {
    Heap heap;
    EdgePtr *heapBuff = malloc(graph->edgeNum * sizeof(EdgePtr));
    for (int i = 0; i < graph->edgeNum; ++i)
        heapBuff[i] = graph->edges + i;
    buildHeap(&heap, heapBuff, graph->edgeNum);

    VertexId *disjSet = malloc(graph->vertexNum * sizeof(VertexId));
    disjointSetInit(disjSet, graph->vertexNum);

    int counter = 0;
    while (heap.size != 0) {
        const EdgePtr edge = *heapTop(&heap);
        heapPop(&heap);

        const int root1 = disjointSetFind(disjSet, edge->vertex1);
        const int root2 = disjointSetFind(disjSet, edge->vertex2);

        if (root1 != root2) {
            outputArray[counter++] = (EdgeId) (edge - graph->edges);
            disjointSetUnion(disjSet, root1, root2);
        }
    }
    if (counter + 1 != graph->vertexNum)
        printf("No spanning tree!\n");

    outputArray[counter] = -1;
    free(heapBuff);
}
