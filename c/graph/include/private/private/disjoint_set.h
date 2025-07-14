#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include "graph/type.h"

typedef GraphId GraphDisjointSet;

GraphDisjointSet *graphDisjointCreate(GraphSize size);

void graphDisjointRelease(GraphDisjointSet *set);

static void graphDisjointUnion(GraphDisjointSet *set, const GraphId class1,
                               const GraphId class2) {
  GraphId *neg_height = set;
  if (neg_height[class1] > neg_height[class2]) {
    set[class1] = class2;
  } else {
    if (neg_height[class1] == neg_height[class2]) --neg_height[class1];
    set[class2] = class1;
  }
}

static GraphId graphDisjointFind(GraphDisjointSet *set, const GraphId id) {
  GraphId cls;
  for (cls = id; set[cls] >= 0; cls = set[cls]);
  for (GraphId i = id; i != cls;) {
    const GraphId next = set[i];
    set[i] = cls;
    i = next;
  }
  return cls;
}

#endif // DISJOINT_SET_H
