#ifndef QUEUE_H
#define QUEUE_H

#include "graph/type.h"

typedef struct {
  GraphSize capacity, size;
  GraphSize front, rear;
  GraphId data[0];
} GraphQueue;

GraphQueue *graphNewQueue(GraphSize capacity);

void graphQueueRelease(GraphQueue *queue);

void graphQueuePush(GraphQueue *queue, GraphId item);

GraphId graphQueuePop(GraphQueue *queue);

static inline void graphQueueClear(GraphQueue *queue) {
  queue->size = queue->front = queue->rear = 0;
}

static inline GraphBool graphQueueEmpty(const GraphQueue *queue) {
  return queue->size == 0;
}

#endif // QUEUE_H
