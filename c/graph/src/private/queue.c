#include "private/queue.h"
#include <stdlib.h>

void graphQueueInit(GraphQueue *queue, const GraphSize capacity) {
  queue->capacity = capacity;
  queue->size = queue->front = queue->rear = 0;
  queue->data = malloc(capacity * sizeof(GraphId));
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

void graphQueueRelease(const GraphQueue *queue) {
  free(queue->data);
}