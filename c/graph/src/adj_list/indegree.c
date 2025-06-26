#include "graph/adj_list/indegree.h"
#include <string.h>

void indegreeInit(const GraphInt indegree[], const GraphSize vertCap,
                  GraphInt **copyIndeg, const QueuePtr queue) {
  queueInit(queue, vertCap);
  *copyIndeg = malloc(vertCap * sizeof(GraphInt));
  memcpy(*copyIndeg, indegree, vertCap * sizeof(GraphInt));

  for (GraphId i = 0; i < vertCap; i++) {
    if (indegree[i] == 0) {
      enqueue(queue, i);
    }
  }
}
