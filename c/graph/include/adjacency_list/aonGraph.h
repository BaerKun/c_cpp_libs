#ifndef AONGRAPH_H
#define AONGRAPH_H

typedef int TimeType;
typedef int NodeId;

#define VERTEX_DATA
struct VertexData {
    TimeType duration;
    TimeType earlyStart;
    TimeType lateStart;
    TimeType slack;
};

#include "graph.h"

typedef Graph AonGraph, *AonGraphPtr;
typedef Vertex ActivityNode, *ActivityNodePtr;
typedef Edge Dependency;

void addActivityNode(AonGraphPtr aonGraph, TimeType duration);

void establishDependency(AonGraphPtr aonGraph, NodeId start, NodeId end);

void buildCriticalPath(AonGraphPtr aonGraph);

void copyPath(AonGraphPtr aonGraph, NodeId copyArray[]);

#endif //AONGRAPH_H
