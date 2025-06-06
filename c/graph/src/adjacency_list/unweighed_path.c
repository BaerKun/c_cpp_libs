#include "adjacency_list/unweighed_path.h"
#include "queue.h"

void buildUnweightedPath(const ListGraphPtr graph, VertexId *parent,
                         const VertexId source, const VertexId target) {
  Queue queue;
  queueInit(&queue, graph->vertexNum);

  for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++)
    parent[vertex] = -1;

  enqueue(&queue, source);
  while (queueEmpty(&queue)) {
    const VertexId vertex = *queueFront(&queue);
    dequeue(&queue);

    for (ListEdgePtr edge = graph->vertices[vertex].outEdges; edge;
         edge = edge->next) {
      const VertexId adjacentVertex = edge->target;

      if (parent[adjacentVertex] == -1) {
        parent[adjacentVertex] = vertex;
        if (adjacentVertex == target) return;

        enqueue(&queue, adjacentVertex);
      }
    }
  }
  queueFreeData(&queue);
}