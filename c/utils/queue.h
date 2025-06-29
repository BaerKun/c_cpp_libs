#ifndef QUEUE_H
#define QUEUE_H

#ifndef QUEUE_ELEM_TYPE
#define QUEUE_ELEM_TYPE int
#endif

#include <stdint.h>
#include <stdlib.h>

typedef QUEUE_ELEM_TYPE QueueElemType_;
typedef struct Queue_ Queue, *QueuePtr;

struct Queue_ {
  QueueElemType_ *data;
  uint64_t capacity, size;
  uint64_t front, rear;
};

static inline void queueInit(const QueuePtr queue, const uint64_t capacity) {
  queue->data = (QueueElemType_ *)malloc(sizeof(QueueElemType_) * capacity);
  queue->front = queue->rear = queue->size = 0;
  queue->capacity = capacity;
}

static inline void enqueue(const QueuePtr queue, QueueElemType_ const element) {
  queue->data[queue->rear] = element;
  if (++queue->rear == queue->capacity) queue->rear = 0;
  ++queue->size;
}

static inline void dequeue(const QueuePtr queue) {
  if (++queue->front == queue->capacity) queue->front = 0;
  --queue->size;
}

static inline void queueClear(const QueuePtr queue) {
  queue->front = queue->rear = queue->size = 0;
}

static inline QueueElemType_ *queueFront(const Queue *const queue) {
  return queue->data + queue->front;
}

static inline int queueEmpty(const Queue *const queue) {
  return queue->size == 0;
}

static inline void queueFreeData(const QueuePtr queue) { free(queue->data); }

#endif // QUEUE_H
