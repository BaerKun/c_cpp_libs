#include <stdio.h>
#include <math.h>
#include "integer.h"

void float2fraction_shift(const float x, int *numerator, int *rightShift, const int maxShift) {
    const int *px = (int *) &x;
    int _numerator = *px & 0x007FFFFF | 0x00800000;
    int _rightShift = 23 - (((*px & 0x7F800000) >> 23) - 127);

    if (*px & 0x80000000) {
        printf("input is negative!");
    }

    while (_rightShift > maxShift || (_numerator & 1) == 0) {
        _numerator >>= 1;
        _rightShift--;
    }

    if (_rightShift < 0) {
        printf("rightShift is negative!");
    }

    *numerator = _numerator;
    *rightShift = _rightShift;
}

void float2fraction_precise(float x, int *numerator, int *denominator, const int maxDeno) {
    const int *const px = (int *) &x,
            exponent = ((*px & 0x7F800000) >> 23) - 127;
    int _maxDeno, isNagative = 0;

    if (exponent >= 23) {
        if (exponent >= 31) {
            printf("input is too large!");
            *denominator = 0;
            return;
        }
        *numerator = x;
        *denominator = 1;
        return;
    }

    if (exponent < -8) {
        if (exponent < -31) {
            printf("input is too small!");
            *numerator = 0;
            return;
        }
        _maxDeno = 0x7fffffff;
    } else {
        _maxDeno = 1 << (23 - exponent);
    }

    if (maxDeno < _maxDeno)
        _maxDeno = maxDeno;

    if (x < 0.f) {
        isNagative = 1;
        x = -x;
    }

    int n = 0, d = 1,
            outNume = 0,
            outDeno = 1;
    float minDiff = INFINITY;

    do {
        float diff1 = fabs(x - (float) n / d);
        while (1) {
            const float diff2 = fabs(x - (float) (n + 1) / d);
            if (diff1 < diff2)
                break;
            diff1 = diff2;
            ++n;
        }

        if (diff1 >= minDiff)
            continue;

        const float gcd = greatestCommonDivisor(d, n);
        if (d / gcd > _maxDeno)
            continue;

        outDeno = d / gcd;
        outNume = n / gcd;
        minDiff = diff1;
    } while (d++ < _maxDeno && minDiff != 0.f);

    *numerator = isNagative ? -outNume : outNume;
    *denominator = outDeno;
}
