#include "graph/adj_list/unweighed_path.h"
#include "queue.h"
#include <string.h>

void unweightedShortest(const Graph *const graph, GraphId predecessor[],
                         const GraphId source, const GraphId target) {
  Queue queue;
  queueInit(&queue, graph->vertNum);
  memset(predecessor, 255, sizeof(GraphId) * graph->vertCap); // -1

  enqueue(&queue, source);
  while (!queueEmpty(&queue)) {
    const GraphId from = *queueFront(&queue);
    dequeue(&queue);

    for (GraphEdgePtr edge = graph->adjList[from]; edge; edge = edge->next) {
      const GraphId to = edge->to;
      if (predecessor[to] == -1) {
        predecessor[to] = from;
        if (to == target) return;
        enqueue(&queue, to);
      }
    }
  }
  queueFreeData(&queue);
}