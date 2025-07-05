#include "graph/iter.h"
#include "private/graph_detail.h"
#include <stdlib.h>
#include <string.h>

void graphIterInit(const Graph *graph, GraphIter *iter) {
  iter->vertCurr = graph->vertMng.iterHead;
  const GraphSize size = graph->vertMng.idMng.range * sizeof(GraphId);
  iter->edgeCurr = malloc(size);
  memcpy(iter->edgeCurr, graph->edgeMng.iterHead, size);
  iter->vertNext = graph->vertMng.idMng.next;
  iter->edgeNext = graph->edgeMng.idMng.next;
  iter->endpts = graph->endpts;
}

void graphIterRelease(const GraphIter *iter) { free(iter->edgeCurr); }

void graphIterResetVert(const Graph *graph, GraphIter *iter) {
  iter->vertCurr = graph->vertMng.iterHead;
}

void graphIterResetEdge(const Graph *graph, const GraphIter *iter,
                        const GraphId from) {
  if (from == INVALID_ID) {
    memcpy(iter->edgeCurr, graph->edgeMng.iterHead, graph->vertMng.idMng.range);
  } else {
    iter->edgeCurr[from] = graph->edgeMng.iterHead[from];
  }
}