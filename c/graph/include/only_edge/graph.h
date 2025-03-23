/*
 * GraphPtr 类型包括 顶点数 和 邻接表数组；
 * 邻接表的表头存储了 路径 和 距离 信息；
 * BuildPath 会修改这些数据；
 * 无效的终点输入， 或 起点->终点的路径 不存在， BuildPath 会遍历所有可到达的顶点；
 * */

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "share/vertex_edge.h"

typedef int VertexId;
typedef int EdgeId;
typedef int WeightType;

typedef struct {
    VertexId vertex1;
    VertexId vertex2;
    EdgeData data;
} Edge, *EdgePtr;

typedef struct Graph {
    int edgeCapacity;
    int edgeNum;
    int vertexNum;
    EdgePtr edges;
} Graph, *GraphPtr;

GraphPtr newGraph(int edgeCapacity, int vertexNum);

void graphAddEdge(GraphPtr graph, VertexId vertex1, VertexId vertex2, EdgeData data);

void graphDestroy(GraphPtr graph);

#endif //GRAPH_GRAPH_H
