#ifndef GRAPH_INIT_INDEGREE_H
#define GRAPH_INIT_INDEGREE_H

#include "private/queue.h"

GraphInt *indegreeInit(const GraphInt indegree[], GraphQueue *queue,
                       GraphSize vertCap);

#endif // GRAPH_INIT_INDEGREE_H
