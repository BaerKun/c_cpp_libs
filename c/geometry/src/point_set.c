#include <string.h>
#include <float.h>

#include "point_set.h"
#include "geom_utils.h"

#define SORT_ELEMENT_TYPE Point2f
#define SORT_LESS_THAN(p, q) (p.x < q.x)

#include "sort.h"

#define STACK_ELEMENT_TYPE Point2f

#include "stack.h"

static void convexHullHelper(const StackPtr stack, const Point2f p) {
    const Point2f *ptr2pt = stack->elements + stack->top - 1;
    Point2f p2 = *ptr2pt;
    Vector2f vec2 = (Vector2f){p.x - p2.x, p.y - p2.y};

    do {
        const Vector2f vec1 = vec2;
        --ptr2pt;
        p2 = *ptr2pt;
        vec2 = (Vector2f){p.x - p2.x, p.y - p2.y};

        if (cross_vec(vec1, vec2) < 0)
            break;

        stackPop(stack);
    } while (stack->top > 2);

    stackPush(stack, p);
}

static void ptsMaxDist_force(Point2f hullpts[], Point2f *outpts, const int nhullpts) {
    float max = 0;

    for (int i = 0; i < nhullpts; ++i) {
        for (int j = i + 1; j < nhullpts; ++j) {
            const float sqrd = sqrdist(hullpts[i], hullpts[j]);
            if (sqrd > max) {
                max = sqrd;
                outpts[0] = hullpts[i];
                outpts[1] = hullpts[j];
            }
        }
    }
}

static float
ptsMinDistHelper_force(const Point2f *ptsleft, const Point2f *ptsright, Point2f *outleft, Point2f *outright, float minsqd) {
    for (const Point2f *p = ptsleft; p != ptsright; ++p) {
        for (const Point2f *q = p + 1; q <= ptsright; ++q) {
            const float sqrd = sqrdist(*p, *q);
            if (sqrd < minsqd) {
                minsqd = sqrd;
                *outleft = *p;
                *outright = *q;
            }
        }
    }
    return minsqd;
}

static float ptsMinDistHelper(Point2f pts[], Point2f outpts[], int left, int right) {
    if (right - left < 8)
        return ptsMinDistHelper_force(pts + left, pts + right, outpts + left, outpts + right, FLT_MAX);

    const int center = (left + right) / 2;
    float midMinSqrd;

    const float leftMinSqrd = ptsMinDistHelper(pts, outpts, left, center);
    const float rightMinSqrd = ptsMinDistHelper(pts, outpts, center + 1, right);

    if (leftMinSqrd < rightMinSqrd) {
        midMinSqrd = leftMinSqrd;
        outpts[right] = outpts[center];
    } else {
        midMinSqrd = rightMinSqrd;
        outpts[left] = outpts[center + 1];
    }

    const float borderWidth = sqrtf(midMinSqrd);
    int borderLeft = center, borderRight = center;

    while (borderLeft > left && pts[center].x - pts[borderLeft - 1].x < borderWidth)
        --borderLeft;

    while (borderRight < right && pts[borderRight + 1].x - pts[center].x < borderWidth)
        ++borderRight;

    return ptsMinDistHelper_force(pts + borderLeft, pts + borderRight, outpts + left, outpts + right, midMinSqrd);
}

void convexHull(Point2f pts[], Point2f hullpts[], int npts, int *nhullpts) {
    if (npts <= 3) {
        *nhullpts = npts;
        memcpy(hullpts, pts, sizeof(Point2f) * npts);
        return;
    }

    Point2f origin = pts[0];

    for (int i = 1; i < npts; ++i) {
        if (pts[i].y < origin.y || (pts[i].y == origin.y && pts[i].x < origin.x))
            origin = pts[i];
    }

    struct {
        float theta;
        int index;
    } *tmp = malloc(npts * sizeof(*tmp));

    for (int i = 0; i < npts; ++i) {
        tmp[i].theta = atan2f(pts[i].y - origin.y, pts[i].x - origin.x);
        tmp[i].index = i;
    }

    sort((Point2f *) tmp, npts);

    const StackPtr stack = newStack(npts);
    stackPush(stack, pts[tmp[0].index]);
    stackPush(stack, pts[tmp[1].index]);
    stackPush(stack, pts[tmp[2].index]);

    for (int i = 3; i < npts; ++i)
        convexHullHelper(stack, pts[tmp[i].index]);

    convexHullHelper(stack, origin);

    *nhullpts = stack->top - 1;
    memcpy(hullpts, stack->elements, sizeof(Point2f) * *nhullpts);

    stackDestroy(stack);
    free(tmp);
}

void convexHullDiameter(Point2f hullpts[], Point2f *outpts, int nhullpts) {
    if (nhullpts <= 10) {
        ptsMaxDist_force(hullpts, outpts, nhullpts);
        return;
    }

    int i = 0, j = 3;
    float max = 0.f;

    Point2f p2 = hullpts[0],
            p3 = hullpts[1],
            p4 = hullpts[2];
    Vector2f v2 = {p4.x - hullpts[1].x, p4.y - hullpts[1].y};
    do {
        ++i;
        const Point2f p1 = p2;
        p2 = hullpts[i == nhullpts ? 0 : i];
        const Vector2f v1 = {p2.x - p1.x, p2.y - p1.y};
        do {
            if (cross_vec(v1, v2) <= 0.f)
                break;

            p3 = p4;
            p4 = hullpts[j == nhullpts ? 0 : j];
            v2 = (Vector2f){p4.x - p3.x, p4.y - p3.y};

            if (j == nhullpts)
                j = 1;
            else
                ++j;
        } while (1);

        const float sqrd = sqrdist(p1, p3);
        if (sqrd > max) {
            max = sqrd;
            outpts[0] = p1;
            outpts[1] = p3;
        }
    } while (i != nhullpts);
}

void ptsMaxDist(Point2f pts[], Point2f *maxPoint, int npts) {
    if (npts <= 10) {
        ptsMaxDist_force(pts, maxPoint, npts);
        return;
    }

    int nhullpts;
    Point2f *hullpts = malloc(npts * sizeof(Point2f));
    if (hullpts == NULL)
        return;

    convexHull(pts, hullpts, npts, &nhullpts);
    convexHullDiameter(hullpts, maxPoint, nhullpts);

    free(hullpts);
}

float ptsMinDist(Point2f pts[], Point2f outpts[2], int npts) {
    Point2f *tmpOutpts = malloc(sizeof(Point2f) * npts);
    if (tmpOutpts == NULL)
        return -1.f;

    sort(pts, npts);

    const float mindist = sqrtf(ptsMinDistHelper(pts, tmpOutpts, 0, npts - 1));

    outpts[0] = tmpOutpts[0];
    outpts[1] = tmpOutpts[npts - 1];

    free(tmpOutpts);
    return mindist;
}

void minEnclosingCircle(Point2f pts[], const int npts, Point2f *center, float *radius) {
    if (npts <= 0)
        return;

    // 优化地遍历所有两点直径圆和三点外接圆，通过判断新点是否在圆内，决定是否更新圆
    // 初始点pts[0]
    *center = pts[0];
    *radius = 0;
    for (int i = 1; i < npts; ++i) {
        // 遍历pts[i](新点)和pts[j]的直径圆
        // 在循环j前判断pts[i]是否在圆内，以避免不必要点计算

        if (dist2f(*center, pts[i]) <= *radius)
            continue;
        *center = midpt(*pts, pts[i]);
        *radius = dist2f(*center, *pts);
        for (int j = 1; j < i; ++j) {
            if (dist2f(*center, pts[j]) <= *radius)
                continue;
            *center = midpt(pts[i], pts[j]);
            *radius = dist2f(*center, pts[i]);

            // 遍历三点外接圆
            for (int k = 0; k < j; ++k) {
                if (dist2f(*center, pts[k]) <= *radius)
                    continue;
                triCircumcircle(pts[i], pts[j], pts[k], center, radius);
            }
        }
    }
}
