#include "graph/adj_list/aoa.h"
#include "private/indegree.h"
#include <stdlib.h>
#include <string.h>

static void forward(const GraphEdgePtr adjList[], GraphInt indegree[],
                    const TimeType duration[], TimeType earlyStart[],
                    GraphQueue *const queue) {
  while (!graphQueueEmpty(queue)) {
    const GraphId node = graphQueuePop(queue);

    for (const GraphEdge *arrow = adjList[node]; arrow; arrow = arrow->next) {
      const GraphId succ = arrow->to; // successor 后继
      if (earlyStart[succ] < earlyStart[node] + duration[node])
        earlyStart[succ] = earlyStart[node] + duration[node];
      if (--indegree[succ] == 0) graphQueuePush(queue, succ);
    }
  }
}

static void backward(const GraphEdgePtr adjList[],
                     const GraphQueue *const queue, const TimeType duration[],
                     const TimeType earlyStart[], TimeType lateStart[],
                     GraphId successor[]) {
  const GraphId *p = queue->data + queue->front;
  do {
    const GraphId node = *--p;
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
  } while (p != queue->data);
}

void criticalPath(const Graph *aoa, const TimeType duration[],
                  const GraphInt indegree[], GraphId successor[],
                  TimeType earlyStart[], TimeType lateStart[]) {
  GraphQueue queue;
  GraphInt *copyIndeg = indegreeInit(indegree, &queue, aoa->vertCap);
  memset(earlyStart, 0x7f, aoa->vertCap * sizeof(TimeType));
  memset(lateStart, 0, aoa->vertCap * sizeof(TimeType));

  forward(aoa->adjList, copyIndeg, duration, earlyStart, &queue);

  const GraphId last = graphQueuePop(&queue);
  lateStart[last] = earlyStart[last];

  backward(aoa->adjList, &queue, duration, earlyStart, lateStart, successor);

  graphQueueRelease(&queue);
  free(copyIndeg);
}
