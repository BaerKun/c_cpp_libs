#ifndef GRAPH_INIT_INDEGREE_H
#define GRAPH_INIT_INDEGREE_H

#include "private/graph_detail.h"
#include "graph/iter.h"
#include "private/queue.h"

void indegreeInit(GraphIter *iter, const GraphInt indegree[],
                  GraphQueue *queue);

void graphMangerInsert(GraphManager *mng, GraphId from, Graph id);



#endif // GRAPH_INIT_INDEGREE_H
