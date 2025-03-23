#include "kalman.h"

float kalmanUpdate(KalmanFilter *filter, const float input, const float measurement) {
    const float x_pred = filter->F * filter->x + filter->B * input;
    const float P_pred = filter->F * filter->P * filter->F + filter->Q;

    const float K = P_pred * filter->H / (filter->H * P_pred * filter->H + filter->R);
    filter->x = x_pred + K * (measurement - filter->H * x_pred);
    filter->P = (1 - K * filter->H) * P_pred;

    return filter->x;
}