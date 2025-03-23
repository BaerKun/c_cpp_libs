#include "matrix/all_pairs_weighted_path.h"
#include <stdlib.h>

void allPairsWeightedPath(const GraphPtr graph) {
    WeightType (*distances)[MAX_VERTEX] = malloc(sizeof(WeightType) * graph->vertexNum * graph->vertexNum);
    VertexId source, target;

    for (source = 0; source < graph->vertexNum; source++) {
        for (target = 0; target < graph->vertexNum; target++){
            distances[source][target] = graph->edges[source][target].data.weight;
        }
    }

    for(VertexId middle = 0; middle < graph->vertexNum; middle++){
        for(source = 0; source < graph->vertexNum; source++){
            if(distances[source][middle] == WIGHT_MAX)
                continue;

            for(target = 0; target < graph->vertexNum; target++){
                if(distances[middle][target] == WIGHT_MAX)
                    continue;

                if(distances[source][middle] + distances[middle][target] < distances[source][target]){
                    distances[source][target] = distances[source][middle] + distances[middle][target];

                    if(graph->edges[source][middle].data.weight != WIGHT_MAX){
                        graph->edges[source][target].path = middle;
                    }
                }
            }
        }
    }

    free(distances);
}