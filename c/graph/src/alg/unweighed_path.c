#include "private/graph_detail.h"
#include "private/queue.h"
#include "graph/iter.h"
#include <string.h>

void unweightedShortest(const Graph *const graph, GraphId predecessor[],
                         const GraphId source, const GraphId target) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  GraphQueue *queue = graphNewQueue(graph->vertNum);
  memset(predecessor, INVALID_ID, sizeof(GraphId) * view->vertRange);

  GraphId id, to;
  graphQueuePush(queue, source);
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (predecessor[to] == -1) {
        predecessor[to] = from;
        if (to == target) return;
        graphQueuePush(queue, to);
      }
    }
  }
  graphIterRelease(iter);
  graphQueueRelease(queue);
}