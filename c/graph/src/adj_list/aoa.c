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
      if (earlyStart[succ] < earlyStart[node] + duration[arrow->id])
        earlyStart[succ] = earlyStart[node] + duration[arrow->id];
      if (--indegree[succ] == 0) graphQueuePush(queue, succ);
    }
  }
}

static void backward(const GraphEdgePtr adjList[], const TimeType duration[],
                     const TimeType earlyStart[], TimeType lateStart[],
                     GraphId successor[], const GraphId *const begin,
                     const GraphId *const end) {
  const GraphId *p = end;
  do {
    const GraphId node = *--p;
    for (const GraphEdge *arrow = adjList[node]; arrow; arrow = arrow->next) {
      const GraphId succ = arrow->to;
      if (lateStart[node] > lateStart[succ] - duration[arrow->id]) {
        lateStart[node] = lateStart[succ] - duration[arrow->id];
        if (lateStart[node] == earlyStart[node]) {
          successor[node] = succ;
          break;
        }
      }
    }
  } while (p != begin);
}

void criticalPath(const Graph *aoa, const TimeType duration[],
                  const GraphInt indegree[], GraphId successor[],
                  TimeType earlyStart[], TimeType lateStart[]) {
  GraphQueue queue;
  GraphInt *copyIndeg = indegreeInit(indegree, &queue, aoa->vertCap);
  memset(successor, 255, aoa->vertCap * sizeof(GraphId));
  memset(earlyStart, 0, aoa->vertCap * sizeof(TimeType));
  memset(lateStart, 0x7f, aoa->vertCap * sizeof(TimeType));

  forward(aoa->adjList, copyIndeg, duration, earlyStart, &queue);

  const GraphId *last = queue.data + aoa->vertCap - 1;
  lateStart[*last] = earlyStart[*last];

  backward(aoa->adjList, duration, earlyStart, lateStart, successor, queue.data,
    last);

  graphQueueRelease(&queue);
  free(copyIndeg);
}
