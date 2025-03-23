#ifndef POINT_VECTOR_H
#define POINT_VECTOR_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct Point2i {
    int x;
    int y;
}Point2i;

typedef struct Point2f {
    float x;
    float y;
}Point2f;

typedef struct Vector3f
{
    float x, y, z;
} Vector3f;


typedef Point2f Vector2f;

#endif //POINT_VECTOR_H
