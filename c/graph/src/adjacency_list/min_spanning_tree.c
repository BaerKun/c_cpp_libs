#include "adjacency_list/min_spanning_tree.h"
#include <stdio.h>
#include <stdlib.h>
#define HEAP_ELEMENT_TYPE int *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"


static void printTreeHelper(const VertexPtr vertices, VertexId parent[], const VertexId root, const int deepth) {
    for (int i = 0; i < deepth; i++)
        printf("\t");
    printf("|-%d\n", root);

    for (EdgePtr edge = vertices[root].outEdges; edge; edge = edge->next)
        if (parent[edge->target] == root)
            printTreeHelper(vertices, parent, edge->target, deepth + 1);
}

void PrimMinSpanningTree(const GraphPtr graph, VertexId parent[], const VertexId root) {
    VertexId vertex;
    char *hasKnown = malloc(graph->vertexNum);
    int *minWeight = malloc(graph->vertexNum * sizeof(int));
    const HeapPtr heap = newHeap(graph->vertexNum);
    parent[root] = root;

    for (vertex = 0; vertex < graph->vertexNum; vertex++) {
        hasKnown[vertex] = 0;
        minWeight[vertex] = WIGHT_MAX;
    }

    heapInsert(heap, minWeight + root);

    while (heap->size) {
        vertex = (VertexId) (heapDeleteMin(heap) - minWeight);
        hasKnown[vertex] = 1;

        for (EdgePtr thisEdge = graph->vertices[vertex].outEdges; thisEdge; thisEdge = thisEdge->next) {
            const VertexId adjacentVertex = thisEdge->target;

            if (!hasKnown[adjacentVertex] && minWeight[adjacentVertex] > thisEdge->data.weight) {
                minWeight[adjacentVertex] = thisEdge->data.weight;
                parent[adjacentVertex] = vertex;
                heapInsert(heap, minWeight + adjacentVertex);
            }
        }
    }

    free(hasKnown);
    free(minWeight);
    heapDestroy(heap);
}

void printTree(const GraphPtr graph, VertexId parent[], const VertexId root) {
    if (root < 0 || root >= graph->vertexNum || parent[root] != root) {
        fputs("printTree: Invalid root vertex!\n", stderr);
        return;
    }

    printTreeHelper(graph->vertices, parent, root, 0);
}
