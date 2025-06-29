#include "graph/adj_list/unweighed_path.h"
#include "private/queue.h"
#include <string.h>

void unweightedShortest(const Graph *const graph, GraphId predecessor[],
                         const GraphId source, const GraphId target) {
  GraphQueue queue;
  graphQueueInit(&queue, graph->vertNum);
  memset(predecessor, 255, sizeof(GraphId) * graph->vertCap); // -1

  graphQueuePush(&queue, source);
  while (!graphQueueEmpty(&queue)) {
    const GraphId from = graphQueuePop(&queue);

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;
      if (predecessor[to] == -1) {
        predecessor[to] = from;
        if (to == target) return;
        graphQueuePush(&queue, to);
      }
    }
  }
  graphQueueRelease(&queue);
}