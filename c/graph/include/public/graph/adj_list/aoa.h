#ifndef AOA_H
#define AOA_H

#include "graph/graph.h"

typedef WeightType TimeType;

void criticalPath(const Graph *aoa, const TimeType duration[],
                  const GraphInt indegree[], GraphId successor[],
                  TimeType earlyStart[], TimeType lateStart[]);

#endif // AOA_H
