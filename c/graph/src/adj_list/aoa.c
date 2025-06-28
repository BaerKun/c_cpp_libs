#include "graph/adj_list/aoa.h"
#include "private/indegree.h"
#include <stdlib.h>
#include <string.h>

static void forward(const GraphEdgePtr adjList[], GraphInt indegree[],
                    const TimeType duration[], TimeType earlyStart[],
                    const QueuePtr queue) {
  while (queueEmpty(queue)) {
    const GraphId node = *queueFront(queue);
    dequeue(queue);

    for (const GraphEdge *arrow = adjList[node]; arrow; arrow = arrow->next) {
      const GraphId succ = arrow->to; // successor 后继
      if (earlyStart[succ] < earlyStart[node] + duration[node])
        earlyStart[succ] = earlyStart[node] + duration[node];
      if (--indegree[succ] == 0) enqueue(queue, succ);
    }
  }
}

static void backward(const GraphEdgePtr adjList[], const GraphId topoSort[],
                     const TimeType duration[], const TimeType earlyStart[],
                     TimeType lateStart[], GraphId successor[],
                     const int size) {
  for (const GraphId *p = topoSort + size - 1; p != topoSort; --p) {
    const GraphId node = *p;
    for (const GraphEdge *arrow = adjList[node]; arrow; arrow = arrow->next) {
      const GraphId succ = arrow->to;
      if (lateStart[node] > lateStart[succ] - duration[node]) {
        lateStart[node] = lateStart[succ] - duration[node];
        if (lateStart[node] == earlyStart[node]) {
          successor[node] = succ;
          break;
        }
      }
    }
  }
}

void criticalPath(const Graph *aoa, const TimeType duration[],
                  const GraphInt indegree[], GraphId successor[],
                  TimeType earlyStart[], TimeType lateStart[]) {
  Queue queue;
  queueInit(&queue, aoa->vertNum);
  GraphInt *copyIndeg = indegreeInit(indegree, &queue, aoa->vertCap);
  memset(earlyStart, 0x7f, aoa->vertCap * sizeof(TimeType));
  memset(lateStart, 0, aoa->vertCap * sizeof(TimeType));

  forward(aoa->adjList, copyIndeg, duration, earlyStart, &queue);

  const GraphId last = *queueFront(&queue);
  dequeue(&queue);
  lateStart[last] = earlyStart[last];

  backward(aoa->adjList, queue.data, duration, earlyStart, lateStart, successor,
           aoa->vertNum);

  queueFreeData(&queue);
  free(copyIndeg);
}
