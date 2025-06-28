#include "private/indegree.h"
#include <string.h>

GraphInt *indegreeInit(const GraphInt indegree[], const QueuePtr queue,
                       const GraphSize vertCap) {
  queueInit(queue, vertCap);
  GraphInt *const copy = malloc(vertCap * sizeof(GraphInt));
  memcpy(copy, indegree, vertCap * sizeof(GraphInt));

  for (GraphId i = 0; i < vertCap; i++) {
    if (indegree[i] == 0) {
      enqueue(queue, i);
    }
  }
  return copy;
}
