#ifndef GRAPH_FIND_ARTICULATION_H
#define GRAPH_FIND_ARTICULATION_H

#include "graph.h"
#include "link_node.h"

/*
 * 割点：若删除该点及其关联的边后，图会被分割成两个或多个不连通的子图
 * 本函数只用在无向图，且连通
 */
void graphFindArticulation(GraphPtr graph, LinkNodePtr *articulations);

#endif //GRAPH_FIND_ARTICULATION_H
