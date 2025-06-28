#ifndef GRAPH_INIT_INDEGREE_H
#define GRAPH_INIT_INDEGREE_H

#include "graph/graph.h"

#define QUEUE_DATA_TYPE GraphId
#include "queue.h"

GraphInt *indegreeInit(const GraphInt indegree[], QueuePtr queue,
                       GraphSize vertCap);

#endif // GRAPH_INIT_INDEGREE_H
