#include "graph/type.h"
#include "private/attribute.h"
#include <stdlib.h>

void graphAddEdgeAttribute(Graph *graph, const char name[16],
                           const GraphSize sizeOfElem) {
  Attribute *attr = malloc(sizeof(Attribute));
  strHash16(name, attr->hash);
  attr->vector = malloc(graph->edgeCap * sizeOfElem);
  attr->next = graph->edgeAttr;
  graph->edgeAttr = attr;
}

void *graphGetEdgeAttribute(const Graph *graph, const char name[16]) {
  GraphId hash[2];
  strHash16(name, hash);
  for (const Attribute *attr = graph->edgeAttr; attr; attr = attr->next) {
    if (hash[0] == attr->hash[0] && hash[1] == attr->hash[1]) {
      return attr->vector;
    }
  }
  return NULL;
}