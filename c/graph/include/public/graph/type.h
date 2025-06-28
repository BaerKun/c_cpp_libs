#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#include <stdint.h>

#define UNREACHABLE ((WeightType)0x7f7f7f7f7f7f7f7f)

typedef int8_t GraphBool;
typedef int64_t GraphId;
typedef int64_t GraphInt;
typedef uint64_t GraphSize;
typedef int64_t WeightType;

typedef struct Graph_ Graph;
typedef struct GraphEdge_ GraphEdge, *GraphEdgePtr;
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

typedef struct {
  GraphId from, to;
} GraphEdgeEndpoint;

#endif // GRAPH_VERTEX_EDGE_H
