#ifndef AOA_GRAPH_H
#define AOA_GRAPH_H

typedef int TimeType;
typedef int NodeId;

#define VERTEX_DATA                                                                                \
  TimeType duration;                                                                               \
  TimeType earlyStart;                                                                             \
  TimeType lateStart;                                                                              \
  TimeType slack;

#include "adjacency_list/graph.h"

typedef ListGraph AonGraph, *AonGraphPtr;
typedef ListVertex ActivityNode, *ActivityNodePtr;
typedef ListEdge Dependency;

void addActivityNode(AonGraphPtr aonGraph, TimeType duration);

void establishDependency(AonGraphPtr aonGraph, NodeId start, NodeId end);

void buildCriticalPath(AonGraphPtr aonGraph);

void copyPath(AonGraphPtr aonGraph, NodeId copyArray[]);

#endif // AOA_GRAPH_H
