#include "adjacency_list/topsort.h"
#include "adjacency_list/init_indegree.h"
#include "queue.h"
#include <stdio.h>

void buildTopPath(const ListGraphPtr graph, VertexId parent[]) {
  int counter = 0;
  int *indegree = malloc(graph->vertexNum * sizeof(int));
  Queue queue;
  queueInit(&queue, graph->vertexNum);

  InitIndegree(graph, indegree, &queue);
  for (VertexId vertex = 0; vertex < graph->vertexNum; vertex++) parent[vertex] = -1;

  while (queueEmpty(&queue)) {
    const VertexId vertex = *queueFront(&queue);
    dequeue(&queue);
    ++counter;

    for (ListEdgePtr thisEdge = graph->vertices[vertex].outEdges; thisEdge; thisEdge = thisEdge->next) {
      const VertexId adjacentVertex = thisEdge->target;
      if (parent[adjacentVertex] == -1) parent[adjacentVertex] = vertex;
      if (!--indegree[adjacentVertex]) enqueue(&queue, adjacentVertex);
    }
  }

  if (counter != graph->vertexNum) fputs("buildTopPath: Has Cycle\n", stderr);

  free(indegree);
  queueFreeData(&queue);
}

void topSort(const ListGraphPtr graph, VertexId sortArray[]) {
  Queue queue;
  queueInit(&queue, graph->vertexNum);

  int *indegree = malloc(graph->vertexNum * sizeof(int));
  InitIndegree(graph, indegree, &queue);

  int counter = 0;
  while (queueEmpty(&queue)) {
    const VertexId vertex = *queueFront(&queue);
    dequeue(&queue);
    sortArray[counter++] = vertex;

    for (ListEdgePtr edge = graph->vertices[vertex].outEdges; edge; edge = edge->next) {
      if (!--indegree[edge->target]) enqueue(&queue, edge->target);
    }
  }

  if (counter != graph->vertexNum) fputs("topSort: Has Cycle\n", stderr);

  free(indegree);
  queueFreeData(&queue);
}
