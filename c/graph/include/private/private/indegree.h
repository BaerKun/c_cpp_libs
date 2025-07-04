#ifndef GRAPH_INIT_INDEGREE_H
#define GRAPH_INIT_INDEGREE_H

#include "private/queue.h"

GraphInt *indegreeInit(const Graph *graph, const GraphInt indegree[],
                       GraphQueue *queue);

#endif // GRAPH_INIT_INDEGREE_H
