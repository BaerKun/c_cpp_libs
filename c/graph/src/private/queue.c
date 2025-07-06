#include "private/queue.h"
#include <stdlib.h>

GraphQueue *graphNewQueue(const GraphSize capacity) {
  GraphQueue *queue = malloc(sizeof(GraphQueue) + capacity * sizeof(GraphId));
  queue->capacity = capacity;
  queue->size = queue->front = queue->rear = 0;
  return queue;
}

void graphQueuePush(GraphQueue *queue, const GraphId item) {
  queue->data[queue->front] = item;
  if (++queue->front == queue->capacity) queue->front = 0;
  ++queue->size;
}

GraphId graphQueuePop(GraphQueue *queue) {
  const GraphId item = queue->data[queue->rear];
  if (++queue->rear == queue->capacity) queue->rear = 0;
  --queue->size;
  return item;
}

void graphQueueRelease(GraphQueue *queue) {
  free(queue);
}