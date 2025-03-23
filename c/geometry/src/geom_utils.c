#include "geom_utils.h"
#include <math.h>

void cartesian2polar_pt(Point2f inpts[], Point2f outpts[], const int npts, const Point2f origin) {
    for(int i = 0; i < npts; ++i) {
        const float dx = inpts[i].x - origin.x;
        const float dy = inpts[i].y - origin.y;
        outpts[i].x = atan2f(dy, dx);
        outpts[i].y = sqrtf(dx * dx + dy * dy);
    }
}

void cartesian2polar_vec(Vector2f inVectors[], Point2f outVectors[], const int nvecs) {
    for(int i = 0; i < nvecs; ++i) {
        const Vector2f vec = inVectors[i];
        outVectors[i].x = atan2f(vec.y, vec.x);
        outVectors[i].y = sqrtf(vec.x * vec.x + vec.y * vec.y);
    }
}

void polar2cartesian_pt(Point2f pts[], Point2f outpts[], const int npts, const Point2f origin) {
    for(int i = 0; i < npts; ++i) {
        const float theta = pts[i].x;
        const float r = pts[i].y;
        outpts[i].x = origin.x + r * cosf(theta);
        outpts[i].y = origin.y + r * sinf(theta);
    }
}

void polar2cartesian_vec(Vector2f invecs[], Vector2f outvecs[], const int nvecs) {
    for(int i = 0; i < nvecs; ++i) {
        const float theta = invecs[i].x;
        const float r = invecs[i].y;
        outvecs[i].x = r * cosf(theta);
        outvecs[i].y = r * sinf(theta);
    }
}

void triCircumcircle(const Point2f pt1, const Point2f pt2, const Point2f pt3, Point2f *center, float *radius){
    const Vector3f a = {pt2.y - pt3.y, pt3.y - pt1.y, pt1.y - pt2.y};
    const Vector3f b = {norm_vec2((Point2f){pt1.x, pt1.y}), norm_vec2((Point2f){pt2.x, pt2.y}), norm_vec2((Point2f){pt3.x, pt3.y})};

    const float d = vec3_dot(a, (Vector3f){pt1.x, pt2.x, pt3.x}) * 2.f;
    center->x = vec3_dot(a, b) / d;
    center->y = vec3_dot(b, (Vector3f){pt3.x - pt2.x, pt1.x - pt3.x, pt2.x - pt1.x}) / d;
    *radius = dist2f(*center, pt1);
}
