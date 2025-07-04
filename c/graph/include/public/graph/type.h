#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#include <stdint.h>

#define UNREACHABLE ((WeightType)0x7f7f7f7f7f7f7f7f)

typedef int8_t GraphBool;
typedef int64_t GraphId;
typedef int64_t GraphInt;
typedef uint64_t GraphSize;
typedef int64_t WeightType;

typedef WeightType TimeType; // aoa
typedef WeightType FlowType; // flow

typedef struct GraphLinkedPath_ GraphLinkedPath;
struct GraphLinkedPath_ {
  GraphLinkedPath *next;
  GraphId id;
};

typedef struct {
  GraphId curr;
  const GraphId *next;
} GraphIter;

typedef struct {
  GraphId id, to;
} GraphEdge;

// graph
typedef struct Attribute_ Attribute;
typedef struct GraphManager_ GraphManager;

typedef struct {
  GraphId from, to;
} GraphEdgeEndpoint;

typedef struct {
  GraphSize vertCap, edgeCap;
  GraphSize vertNum, edgeNum;
  GraphEdgeEndpoint *endpts;
  GraphManager *manager;
  Attribute *vertAttr, *edgeAttr;
} Graph;

#endif // GRAPH_VERTEX_EDGE_H
