#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#ifndef QUEUE_DATA_TYPE
#define QUEUE_DATA_TYPE int
#endif

typedef struct Queue_ Queue, *QueuePtr;

struct Queue_ {
    QUEUE_DATA_TYPE *data;
    int front;
    int rear;
    int size;
    int capacity;
};

static inline void queueInit(const QueuePtr queue, const int capacity){
    queue->data = (QUEUE_DATA_TYPE *)malloc(sizeof(QUEUE_DATA_TYPE) * capacity);
    queue->front = queue->rear = queue->size = 0;
    queue->capacity = capacity;
}

static inline void enqueue(const QueuePtr queue, QUEUE_DATA_TYPE const element){
    queue->data[queue->rear] = element;
    if(++queue->rear == queue->capacity)
        queue->rear = 0;
    queue->size++;
}

static inline QUEUE_DATA_TYPE dequeue(const QueuePtr queue){
    QUEUE_DATA_TYPE const front = queue->data[queue->front];
    if(++queue->front == queue->capacity)
        queue->front = 0;
    queue->size--;
    return front;
}

static inline void queueClear(const QueuePtr queue){
    queue->front = queue->rear = queue->size = 0;
}

static inline void queueFreeData(const QueuePtr queue){
    free(queue->data);
}

#endif //QUEUE_H
