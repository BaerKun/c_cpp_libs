#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void float2fraction_shift(float x, int *numerator, int *rightShift, int maxShift);

int greatestCommonDivisor(int a, int b);

int integerPartition(int x);

void float2fraction_precise(float x, int *numerator, int *denominator, const int maxDeno);

// 单次调用，若输出0，则必是对的；若输出1，正确率 > 75%
int primalityTest(unsigned n, unsigned numTest);


#endif //ARITHMETIC_H
