#include "graph/graph.h"
#include "private/attribute.h"
#include "private/id_manager.h"
#include <stdlib.h>
#include <string.h>

void graphInit(Graph *const graph, const GraphSize vertCap,
               const GraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  graph->endpts = malloc(edgeCap * sizeof(GraphEdgeEndpoint));
  graph->manager = idManagerCreate(vertCap, edgeCap);
  graph->vertAttr = graph->edgeAttr = NULL;
}

void graphDestroy(const Graph *const graph) {
  const GraphEdgePtr *const end = graph->adjList + graph->vertCap;
  for (const GraphEdgePtr *ptr = graph->adjList; ptr != end; ptr++) {
    for (GraphEdgePtr edge = *ptr, next; edge; edge = next) {
      next = edge->next;
      free(edge);
    }
  }
  destroyAttributeList(graph->vertAttr);
  destroyAttributeList(graph->edgeAttr);
}

void graphGetIndegree(const Graph *graph, GraphInt indegree[]) {
  memset(indegree, 0, graph->vertCap * sizeof(GraphInt));
  for (GraphId from = 0; from < graph->vertCap; ++from) {
    for (const GraphEdge *edge = graph->adjList[from]; edge;
         edge = edge->next) {
      indegree[edge->to]++;
    }
  }
}

