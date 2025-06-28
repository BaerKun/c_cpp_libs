#include "graph/graph.h"
#include "private/edge.h"
#include <stdlib.h>
#include <string.h>

void graphInit(Graph *const graph, const GraphSize vertCap,
               const GraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  graph->adjList = calloc(vertCap, sizeof(GraphEdgePtr));
  graph->vertAttr = graph->edgeAttr = NULL;
}

static void destroyAttributeList(Attribute *attr) {
  for (Attribute *next; attr; attr = next) {
    next = attr->next;
    free(attr->vector);
    free(attr);
  }
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

GraphId graphAddVertex(Graph *const graph) { return graph->vertNum++; }

GraphId graphAddEdge(Graph *const graph, const GraphId from, const GraphId to,
                     const GraphBool directed) {
  const GraphId id = graph->edgeNum++;
  const GraphEdgePtr edge = malloc(sizeof(GraphEdge));
  edge->id = id;
  edge->to = to;
  edgeInsert(graph->adjList + from, edge);

  if (!directed) {
    const GraphEdgePtr reverse = malloc(sizeof(GraphEdge));
    reverse->id = id;
    reverse->to = from;
    edgeInsert(graph->adjList + to, reverse);
  }
  return id;
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

static void strHash16(const char str[16], GraphId hash[2]) {
  hash[0] = hash[1] = 0;
  strncpy((char *)hash, str, 16);
}

void graphAddEdgeAttribute(Graph *graph, const char name[16],
                           const GraphSize sizeOfElem) {
  Attribute *attr = malloc(sizeof(Attribute));
  strHash16(name, attr->hash);
  attr->vector = malloc(graph->edgeCap * sizeOfElem);
  attr->next = graph->edgeAttr;
  graph->edgeAttr = attr;
}

void *graphGetEdgeAttribute(const Graph *graph, const char name[16]) {
  GraphId hash[2];
  strHash16(name, hash);
  for (const Attribute *attr = graph->edgeAttr; attr; attr = attr->next) {
    if (hash[0] == attr->hash[0] && hash[1] == attr->hash[1]) {
      return attr->vector;
    }
  }
  return NULL;
}