#include "adjacency_list/init_indegree.h"

void InitIndegree(const ListGraphPtr graph, int *indegree, const QueuePtr queue) {
  ListVertexPtr vertex = graph->vertices;
  const ListVertexPtr end = vertex + graph->vertNum;
  int *dst = indegree;

  while (vertex != end) {
    *dst = vertex->indegree;
    if (*dst == 0) enqueue(queue, vertex - graph->vertices);

    ++dst;
    ++vertex;
  }
}
