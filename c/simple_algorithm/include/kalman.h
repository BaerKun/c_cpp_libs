#ifndef KALMAN_H
#define KALMAN_H

typedef struct {
    float F, B, H, Q, R, P, x;
} KalmanFilter;

#define KALMAN_INIT(F, B, H, Q, R) (KalmanFilter) { F, B, H, Q, R, 1.f, 0.f }

float kalmanUpdate(KalmanFilter *filter, const float input, const float measurement);

#endif //KALMAN_H
