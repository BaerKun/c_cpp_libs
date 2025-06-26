#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "graph/type.h"

typedef struct GraphEdge_ GraphEdge, *GraphEdgePtr;
typedef struct Graph_ Graph;
typedef struct Attribute_ Attribute;

struct Attribute_ {
  GraphId hash[2];
  Attribute *next;
  void *vector;
};

struct GraphEdge_ {
  GraphId id;
  GraphId to;
  GraphEdgePtr next;
};

struct Graph_ {
  GraphSize vertCap, edgeCap;
  GraphSize vertNum, edgeNum;
  GraphEdgePtr *adjList;
  Attribute *vertAttr, *edgeAttr;
};

void graphInit(Graph *graph, GraphSize vertCap, GraphSize edgeCap);

void graphDestroy(const Graph *graph);

GraphId graphAddVertex(Graph *graph);

GraphId graphAddEdge(Graph *graph, GraphId from, GraphId to, GraphBool directed);

void *graphAddEdgeAttribute(Graph *graph, const char name[16],
                            GraphSize sizeOfElem);

void *graphGetEdgeAttribute(const Graph *graph, const char name[16]);

#endif // GRAPH_GRAPH_H