#include "only_edge/min_spanning_tree.h"
#include "disjSet.h"

#define HEAP_ELEMENT_TYPE EdgePtr
#define HEAP_LESS_THAN(a, b) ((a)->data.weight < (b)->data.weight)
#include "heap.h"

#include <stdio.h>


void KruskalMinSpanningTree(const GraphPtr graph, EdgeId outputArray[]) {
    int counter = 0;
    const HeapPtr heap = newHeap(graph->edgeNum);
    VertexId *disjSet = malloc(graph->vertexNum * sizeof(VertexId));
    EdgePtr edge = graph->edges + graph->edgeNum - 1;

    for (EdgePtr *heapElement = heap->prev + 1; edge >= graph->edges; --edge, ++heapElement)
        *heapElement = edge;

    buildHeap(heap, graph->edgeNum);

    for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++)
        disjSet[vertex] = -1;

    while (heap->size != 0) {
        edge = heapDeleteMin(heap);
        const ClassId root1 = disjSetFind(disjSet, edge->vertex1);
        const ClassId root2 = disjSetFind(disjSet, edge->vertex2);

        if (root1 != root2) {
            outputArray[counter++] = (EdgeId) (edge - graph->edges);
            disjSetUnion(disjSet, root1, root2);
        }
    }
    if (counter + 1 != graph->vertexNum)
        printf("No spanning tree!\n");

    outputArray[counter] = -1;
    heapDestroy(heap);
}
