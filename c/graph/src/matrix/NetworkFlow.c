#include "matrix/NetworkFlow.h"
#include <stdlib.h>
#include <string.h>
#include "queue.h"


NetworkPtr CopyNetwork(const NetworkPtr network) {
    const NetworkPtr copyNetwork = malloc(sizeof(Network));
    memcpy(copyNetwork, network, sizeof(Network));

    copyNetwork->edges = malloc(network->capacity * MAX_VERTEX * sizeof(Edge));

    memcpy(copyNetwork->edges, network->edges, network->capacity * MAX_VERTEX * sizeof(Edge));

    return copyNetwork;
}

static int
BFS(const NetworkPtr network, const QueuePtr queue, const VertexId source, const VertexId target, VertexId path[],
    FlowType *singlrPathFlow) {
    Edge (*edges)[MAX_VERTEX] = network->edges;
    const int vertexNum = network->vertexNum;

    for (VertexId thisVertex = 0; thisVertex < vertexNum; thisVertex++)
        path[thisVertex] = -1;

    path[source] = source;
    enqueue(queue, source);
    while (queue->front != queue->rear) {
        const VertexId thisVertex = dequeue(queue);

        for (VertexId outgoingVertex = 0; outgoingVertex < vertexNum; outgoingVertex++) {
            if (edges[thisVertex][outgoingVertex].data.capacity && path[outgoingVertex] == -1) {
                path[outgoingVertex] = thisVertex;

                if (edges[thisVertex][outgoingVertex].data.capacity < edges[path[thisVertex]][thisVertex].data.flow)
                    edges[thisVertex][outgoingVertex].data.flow = edges[thisVertex][outgoingVertex].data.capacity;
                else
                    edges[thisVertex][outgoingVertex].data.flow = edges[path[thisVertex]][thisVertex].data.flow;

                if (outgoingVertex == target) {
                    *singlrPathFlow = edges[thisVertex][outgoingVertex].data.flow;
                    return 1;
                }

                enqueue(queue, outgoingVertex);
            }
        }
    }
    return 0;
}

FlowType EdmondKarpMaxFlow(const NetworkPtr network, const VertexId source, const VertexId sink) {
    FlowType singlrPathFlow;
    const QueuePtr queue = newQueue(network->vertexNum);
    const NetworkPtr residualNetwork = CopyNetwork(network);
    VertexId *path = malloc(network->vertexNum * sizeof(VertexId));

    residualNetwork->edges[source][source].data.flow = FLOW_MAX;
    while (BFS(residualNetwork, queue, source, sink, path, &singlrPathFlow)) {
        network->edges[sink][sink].data.flow += singlrPathFlow;
        for (VertexId thisVertex = sink; thisVertex != source; thisVertex = path[thisVertex]) {
            const VertexId incomingVertex = path[thisVertex];

            if (network->edges[incomingVertex][thisVertex].data.capacity)
                network->edges[incomingVertex][thisVertex].data.flow += singlrPathFlow;
            else
                network->edges[incomingVertex][thisVertex].data.flow -= singlrPathFlow;

            residualNetwork->edges[incomingVertex][thisVertex].data.flow = 0;
            residualNetwork->edges[incomingVertex][thisVertex].data.capacity -= singlrPathFlow;
            residualNetwork->edges[thisVertex][incomingVertex].data.capacity += singlrPathFlow;
        }
    }

    free(path);
    queue_destroy(queue);
    deleteGraph(residualNetwork);
    return network->edges[sink][sink].data.flow;
}
