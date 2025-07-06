#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/utils.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  GraphIter *iter;
  GraphQueue *queue;
  GraphInt *indegree;
  const TimeType *duration;
  TimeType *earlyStart, *lateStart;
  GraphId *successor;
} Package;

static void forward(const Package *const pkg) {
  GraphId id, to;
  while (!graphQueueEmpty(pkg->queue)) {
    const GraphId from = graphQueuePop(pkg->queue);
    while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
      if (pkg->earlyStart[to] < pkg->earlyStart[from] + pkg->duration[id])
        pkg->earlyStart[to] = pkg->earlyStart[from] + pkg->duration[id];
      if (--pkg->indegree[to] == 0) graphQueuePush(pkg->queue, to);
    }
  }
}

static void backward(const Package *pkg, const GraphId *const begin,
                     const GraphId *const end) {
  const GraphId *p = end;
  GraphId id, to;
  do {
    const GraphId from = *--p;
    while (graphIterNextEdge(pkg->iter, from, &id, &to)) {
      if (pkg->lateStart[from] > pkg->lateStart[to] - pkg->duration[id]) {
        pkg->lateStart[from] = pkg->lateStart[to] - pkg->duration[id];
        if (pkg->lateStart[from] == pkg->earlyStart[from]) {
          pkg->successor[from] = to;
          break;
        }
      }
    }
  } while (p != begin);
}

static void init(Package *pkg, const Graph *aoa, const GraphInt indegree[]) {
  const GraphSize vertRange = aoa->vertMng.range;

  pkg->iter = graphGetIter(aoa);
  pkg->queue = graphNewQueue(vertRange);
  indegreeInit(pkg->iter, pkg->indegree, pkg->queue);
  graphIterResetEdge(aoa, pkg->iter, INVALID_ID);

  pkg->indegree = malloc(vertRange * sizeof(GraphInt));
  memcpy(pkg->indegree, indegree, vertRange * sizeof(GraphInt));
  memset(pkg->earlyStart, 0, vertRange * sizeof(TimeType));
  memset(pkg->lateStart, UNREACHABLE, vertRange * sizeof(TimeType));
  memset(pkg->successor, INVALID_ID, vertRange * sizeof(GraphId));
}

void criticalPath(const Graph *aoa, const GraphInt indegree[],
                  const TimeType duration[], GraphId successor[],
                  TimeType earlyStart[], TimeType lateStart[]) {
  Package pkg;
  pkg.duration = duration;
  pkg.earlyStart = earlyStart;
  pkg.lateStart = lateStart;
  pkg.successor = successor;
  init(&pkg, aoa, indegree);

  forward(&pkg);
  graphIterResetEdge(aoa, pkg.iter, INVALID_ID);

  const GraphId *last = pkg.queue->data + aoa->vertNum - 1;
  lateStart[*last] = earlyStart[*last];

  backward(&pkg, pkg.queue->data, last);

  free(pkg.indegree);
  graphIterRelease(pkg.iter);
  graphQueueRelease(pkg.queue);
}
