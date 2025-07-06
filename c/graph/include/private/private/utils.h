#ifndef GRAPH_INIT_INDEGREE_H
#define GRAPH_INIT_INDEGREE_H

#include "graph/iter.h"
#include "private/queue.h"

void indegreeInit(GraphIter *iter, const GraphInt indegree[],
                  GraphQueue *queue);

#endif // GRAPH_INIT_INDEGREE_H
