#include "../../include/ana/aoa_graph.h"
#include "../../include/adjacency_list/init_indegree.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

void addActivityNode(const AonGraphPtr aonGraph, const TimeType duration) {
  if (aonGraph->vertexNum == aonGraph->vertCap) {
    aonGraph->vertCap *= 2;

    aonGraph->vertices = realloc(aonGraph->vertices, sizeof(ActivityNode) * aonGraph->vertCap);
    if (aonGraph->vertices == NULL) {
      fputs("AddActivityNode: realloc failed\n", stderr);
      return;
    }
  }

  aonGraph->vertices[aonGraph->vertexNum++].data.duration = duration;
}

void establishDependency(const AonGraphPtr aonGraph, const NodeId start, const NodeId end) {
  graphAddEdge(aonGraph, start, end, (EdgeData){0}, 0);
}

static void buildCirticalPathForward(ActivityNode nodes[], const QueuePtr queue, int indegree[]) {
  while (queue->front != queue->rear) {
    const int i = *queueFront(queue);
    dequeue(queue);
    const ActivityNodePtr node = nodes + i;
    for (const Dependency *dependency = node->outEdges; dependency; dependency = dependency->next) {
      const NodeId successorId = dependency->target;
      const ActivityNodePtr successor = nodes + successorId;

      if (successor->data.earlyStart < node->data.earlyStart + node->data.duration)
        successor->data.earlyStart = node->data.earlyStart + node->data.duration;
      if (--indegree[successorId] == 0) enqueue(queue, successorId);
    }
  }
}

static void buildCriticalPathBackward(ActivityNode nodes[], const int topSort[], const int size) {
  for (const int *p = topSort + size - 1; p > topSort; p--) {
    const ActivityNodePtr node = nodes + *p;
    for (const Dependency *dependency = node->outEdges; dependency; dependency = dependency->next) {
      const NodeId successorId = dependency->target;
      const ActivityNodePtr successor = nodes + successorId;
      if (node->data.lateStart > successor->data.lateStart - node->data.duration) {
        node->data.lateStart = successor->data.lateStart - node->data.duration;
        if (node->data.earlyStart == node->data.lateStart && successor->data.slack == 0)
          node->path = successorId;
      }
    }
    node->data.slack = node->data.lateStart - node->data.earlyStart;
  }
}

void buildCriticalPath(const AonGraphPtr aonGraph) {
  Queue queue;
  queueInit(&queue, aonGraph->vertexNum);
  int *indegree = malloc(sizeof(int) * aonGraph->vertexNum);

  InitIndegree(aonGraph, indegree, &queue);

  if (queueEmpty(&queue)) {
    fputs("BuildCriticalPath:HasCycle\n", stderr);
    goto END;
  }

  buildCirticalPathForward(aonGraph->vertices, &queue, indegree);

  const ActivityNodePtr lastOne = aonGraph->vertices + queue.data[queue.rear - 1];
  lastOne->data.lateStart = lastOne->data.earlyStart;
  lastOne->data.slack = 0;

  buildCriticalPathBackward(aonGraph->vertices, queue.data, aonGraph->vertexNum);

END:
  queueFreeData(&queue);
  free(indegree);
}

void copyPath(const AonGraphPtr aonGraph, NodeId copyArray[]) {
  NodeId nodeId;

  for (nodeId = 0; nodeId < aonGraph->vertexNum; nodeId++) {
    if (aonGraph->vertices[nodeId].indegree == 0) break;
  }

  for (int counter = 0; nodeId != -1; nodeId = aonGraph->vertices[nodeId].path)
    copyArray[counter++] = nodeId;
}
