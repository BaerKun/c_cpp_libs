#ifndef GRAPH_DETAIL_H
#define GRAPH_DETAIL_H

#include "graph/type.h"

typedef struct {
  GraphSize range;
  GraphId freeHead;
  GraphId *next;
} IdManager;

typedef struct {
  GraphBool directed;
  GraphId *iterHead;
  GraphId *buff;
  IdManager idMng;
} GraphEdgeManager;

typedef struct {
  GraphId iterHead;
  IdManager idMng;
} GraphVertManager;

typedef struct Attribute_ Attribute;
struct Attribute_ {
  uint64_t hash[2];
  Attribute *next;
  void *vector;
};

struct Graph_ {
  GraphSize vertCap, edgeCap;
  GraphSize vertNum, edgeNum;
  GraphEdgeEndpoint *endpts;
  GraphVertManager vertMng;
  GraphEdgeManager edgeMng;
  Attribute *vertAttr, *edgeAttr;
};

#endif // GRAPH_DETAIL_H
