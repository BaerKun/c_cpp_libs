#ifndef GRAPH_ALG_H
#define GRAPH_ALG_H

#include "graph/type.h"

void criticalPath(const Graph *aoa, const GraphInt indegree[],
                  const TimeType duration[], GraphId successor[],
                  TimeType earlyStart[], TimeType lateStart[]);

void EulerCircuit(const Graph *graph, GraphLinkedNode **path, GraphId src);

void EulerPath(const Graph *graph, GraphLinkedNode **path, GraphId src,
               GraphId dst);

/*
 * 割点：若删除该点及其关联的边后，图会被分割成两个或多个不连通的子图
 * 本函数只用在无向图，且连通
 */
void graphFindArticulation(const Graph *graph, GraphLinkedNode **articulations);

// 强连接分支 Strongly Connected Component
void graphFindScc(const Graph *graph, GraphId connectionId[]);

FlowType EdmondsKarpMaxFlow(const Graph *network, const FlowType capacity[],
                            FlowType flow[], GraphId source, GraphId sink);

void PrimMinSpanningTree(const Graph *graph, const WeightType weight[],
                         GraphId predecessor[], GraphId root);

void KruskalMinSpanningTree(const GraphLinkedNode *set,
                            const WeightType weight[], GraphSize edgeNum,
                            GraphSize vertNum, GraphId tree[]);

void topoSort(const Graph *graph, const GraphInt indegree[], GraphId sort[]);

void topoPath(const Graph *graph, const GraphInt indegree[],
              GraphId predecessor[]);

void unweightedShortest(const Graph *graph, GraphId predecessor[],
                        GraphId source, GraphId target);

void DijkstraShortest(const Graph *graph, const WeightType weight[],
                      GraphId predecessor[], GraphId source, GraphId target);

void BellmanFordShortest(const Graph *graph, const WeightType weight[],
                         GraphId predecessor[], GraphId source);

#endif // GRAPH_ALG_H
