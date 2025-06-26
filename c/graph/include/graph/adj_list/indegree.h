#ifndef GRAPH_INIT_INDEGREE_H
#define GRAPH_INIT_INDEGREE_H

#include "graph/adj_list/graph.h"
#include "queue.h"

void indegreeInit(const GraphInt indegree[], GraphSize vertCap,
                  GraphInt **copyIndeg, QueuePtr queue);

#endif // GRAPH_INIT_INDEGREE_H
