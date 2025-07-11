#ifndef GRAPH_DETAIL_H
#define GRAPH_DETAIL_H

#include "graph/type.h"

#define UNREACHABLE_BYTE 0x7f

typedef struct {
  GraphSize vertRange;
  GraphId vertHead, *vertNext;

  GraphBool directed;
  GraphSize edgeRange;
  GraphId *edgeHead, *edgeNext;
  GraphEndpoint *endpts;
} GraphView; // 最小图信息

struct GraphIter_ {
  const GraphView *view;
  GraphId vertCurr;
  GraphId edgeCurr[0];
};

typedef struct {
  GraphId vertFree, edgeFree;
  GraphView view;
} GraphManager;

typedef struct Attribute_ Attribute;
struct Attribute_ {
  uint64_t hash[2];
  Attribute *next;
  void *vector;
};

struct Graph_ {
  GraphSize vertCap, edgeCap;
  GraphSize vertNum, edgeNum;
  GraphManager manager;
  Attribute *vertAttr, *edgeAttr;
};

#define VIEW(graph) (&(graph)->manager.view)

GraphIter *graphIterFromView(const GraphView *view);

#endif // GRAPH_DETAIL_H
