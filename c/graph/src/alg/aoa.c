#include "graph/graph.h"
#include "private/indegree.h"
#include <stdlib.h>
#include <string.h>

static void forward(const Graph *const aoa, GraphInt indegree[],
                    const TimeType duration[], TimeType earlyStart[],
                    GraphQueue *const queue) {
  GraphIter iter;
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);
    geIterInit(aoa, &iter, from);
    while (!ghIterEnd(&iter)) {
      const GraphEdge arrow = geIterCurr(aoa, &iter);
      if (earlyStart[arrow.to] < earlyStart[from] + duration[arrow.id])
        earlyStart[arrow.to] = earlyStart[from] + duration[arrow.id];
      if (--indegree[arrow.to] == 0) graphQueuePush(queue, arrow.to);
    }
  }
}

static void backward(const Graph *const aoa, const TimeType duration[],
                     const TimeType earlyStart[], TimeType lateStart[],
                     GraphId successor[], const GraphId *const begin,
                     const GraphId *const end) {
  GraphIter iter;
  const GraphId *p = end;
  do {
    const GraphId from = *--p;
    geIterInit(aoa, &iter, from);
    while (!ghIterEnd(&iter)) {
      const GraphEdge arrow = geIterCurr(aoa, &iter);
      if (lateStart[from] > lateStart[arrow.to] - duration[arrow.id]) {
        lateStart[from] = lateStart[arrow.to] - duration[arrow.id];
        if (lateStart[from] == earlyStart[from]) {
          successor[from] = arrow.to;
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
  GraphSize vertRange;
  graphGetIdRange(aoa, &vertRange, NULL);
  GraphInt *copyIndeg = indegreeInit(aoa, indegree, &queue);
  memset(successor, 255, vertRange * sizeof(GraphId));
  memset(earlyStart, 0, vertRange * sizeof(TimeType));
  memset(lateStart, 0x7f, vertRange * sizeof(TimeType));

  forward(aoa, copyIndeg, duration, earlyStart, &queue);

  const GraphId *last = queue.data + vertRange - 1;
  lateStart[*last] = earlyStart[*last];

  backward(aoa, duration, earlyStart, lateStart, successor, queue.data, last);

  free(copyIndeg);
  graphQueueRelease(&queue);
}
