#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#ifndef VERTEX_DATA_TYPE
#define VERTEX_DATA_TYPE int
#endif

typedef VERTEX_DATA_TYPE VertexData;

#ifndef EDGE_DATA_TYPE
#define EDGE_DATA_TYPE int

typedef int WeightType;

struct EdgeData {
  WeightType weight;
};

typedef struct EdgeData EdgeData;

#ifndef WEIGHT_FOR_WEIGHTED_PATH
#define WEIGHT_FOR_WEIGHTED_PATH weight
#endif

#endif

#endif // GRAPH_VERTEX_EDGE_H
