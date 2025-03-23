#ifndef ALL4ALGORITHM_POINT_SET_H
#define ALL4ALGORITHM_POINT_SET_H

#include "geometry.h"

// 当点的数量较多npts>10000时，会出现bug，可能是精度问题
void convexHull(Point2f pts[], Point2f hullpts[], int npts, int *nhullpts);

void convexHullDiameter(Point2f hullpts[], Point2f *outpts, int nhullpts);

void ptsMaxDist(Point2f pts[], Point2f *maxPoint, int npts);

float ptsMinDist(Point2f pts[], Point2f outpts[2], int npts);

void minEnclosingCircle(Point2f pts[], int npts, Point2f *center, float *radius);

#endif //ALL4ALGORITHM_POINT_SET_H
