#include "private/indegree.h"
#include <string.h>
#include <stdlib.h>

GraphInt *indegreeInit(const GraphInt indegree[], GraphQueue *const queue,
                       const GraphSize vertCap) {
  graphQueueInit(queue, vertCap);
  GraphInt *const copy = malloc(vertCap * sizeof(GraphInt));
  memcpy(copy, indegree, vertCap * sizeof(GraphInt));

  for (GraphId i = 0; i < vertCap; i++) {
    if (indegree[i] == 0) {
      graphQueuePush(queue, i);
    }
  }
  return copy;
}
