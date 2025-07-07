#ifndef GRAPH_ITER_H
#define GRAPH_ITER_H

#include "type.h"

GraphIter *graphGetIter(const Graph *graph);
void graphIterRelease(GraphIter *iter);
void graphIterResetVert(GraphIter *iter);
// 重置迭代边，INVALID_ID -> 重置全部
void graphIterResetEdge(GraphIter *iter, GraphId from);

void graphIterCurr(const GraphIter *iter, GraphId *from, GraphId *id,
                   GraphId *to);
GraphBool graphIterNextVert(GraphIter *iter, GraphId *id);
GraphBool graphIterNextEdge(GraphIter *iter, GraphId from, GraphId *id,
                            GraphId *to);

#endif // GRAPH_ITER_H
