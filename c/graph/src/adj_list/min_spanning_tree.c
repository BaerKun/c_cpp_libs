#include "graph/adj_list/min_spanning_tree.h"
#include <stdio.h>
#include <stdlib.h>

#define HEAP_DATA_TYPE int *
#define HEAP_LESS_THAN(a, b) (*a < *b)
#include "heap.h"

static void printTreeHelper(const ListVertexPtr vertices, VertexId parent[], const VertexId root,
                            const int depth) {
  for (int i = 0; i < depth; i++) printf("\t");
  printf("|-%d\n", root);

  for (GraphEdgePtr edge = vertices[root].outEdges; edge; edge = edge->next)
    if (parent[edge->target] == root) printTreeHelper(vertices, parent, edge->target, depth + 1);
}

void PrimMinSpanningTree(const ListGraphPtr graph, VertexId parent[], const VertexId root) {
  char *hasKnown = malloc(graph->vertNum);
  int *minWeight = malloc(graph->vertNum * sizeof(int));
  Heap heap;
  heapInit(&heap, graph->vertNum);
  parent[root] = root;

  for (VertexId vertex = 0; vertex < graph->vertNum; vertex++) {
    hasKnown[vertex] = 0;
    minWeight[vertex] = UNREACHABLE;
  }

  heapPush(&heap, minWeight + root);

  while (heap.size) {
    const VertexId vertex = (VertexId)(*heapTop(&heap) - minWeight);
    heapPop(&heap);
    hasKnown[vertex] = 1;

    for (GraphEdgePtr thisEdge = graph->vertices[vertex].outEdges; thisEdge;
         thisEdge = thisEdge->next) {
      const VertexId adjacentVertex = thisEdge->target;

      if (!hasKnown[adjacentVertex] && minWeight[adjacentVertex] > thisEdge->tail->weight) {
        minWeight[adjacentVertex] = thisEdge->tail->weight;
        parent[adjacentVertex] = vertex;
        heapPush(&heap, minWeight + adjacentVertex);
      }
    }
  }

  free(hasKnown);
  free(minWeight);
  heapFreeData(&heap);
}

void printTree(const ListGraphPtr graph, VertexId parent[], const VertexId root) {
  if (root < 0 || root >= graph->vertNum || parent[root] != root) {
    fputs("printTree: Invalid root vertex!\n", stderr);
    return;
  }

  printTreeHelper(graph->vertices, parent, root, 0);
}
