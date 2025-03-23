#include "arith_utils.h"

#define POWER_HELPER()      \
    while (exponent) {      \
        if (exponent & 1)   \
            out *= base;    \
        exponent >>= 1;     \
        base *= base;       \
    }

int poweri(int base, int exponent) {
    if (exponent < 0)
        return 0;

    int out = 1;
    POWER_HELPER();
    return out;
}

long long powerl(long long base, int exponent) {
    if (exponent < 0)
        return 0;

    long long out = 1;
    POWER_HELPER();
    return out;
}

float powerf(float base, int exponent) {
    const int minus = exponent < 0;
    if(minus)
        exponent = -exponent;

    float out = 1;
    POWER_HELPER();

    if(minus)
        out = 1 / out;
    return out;
}

double powerd(double base, int exponent) {
    const int minus = exponent < 0;
    if (minus)
        exponent = -exponent;

    double out = 1;
    POWER_HELPER();

    if(minus)
        out = 1 / out;
    return out;
}

int sqrti(const int n) {
    if(n < 0)
        return -1;
    if(n == 0)
        return 0;

    int x, y = n;
    do {
        x = y;
        y = (x + n / x) >> 1;
    } while (y < x);

    return x;
}