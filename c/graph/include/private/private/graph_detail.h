#ifndef GRAPH_DETAIL_H
#define GRAPH_DETAIL_H

#include "graph/type.h"

typedef struct {
  GraphSize range;
  GraphId freeHead;
  GraphId *iterHead;
  GraphId *next;
  GraphId *buff;
} GraphManager;

typedef struct Attribute_ Attribute;
struct Attribute_ {
  uint64_t hash[2];
  Attribute *next;
  void *vector;
};

struct Graph_ {
  GraphBool directed;
  GraphSize vertCap, edgeCap;
  GraphSize vertNum, edgeNum;
  GraphEdgeEndpoint *endpts;
  GraphManager vertMng;
  GraphManager edgeMng;
  Attribute *vertAttr, *edgeAttr;
};

#endif // GRAPH_DETAIL_H
