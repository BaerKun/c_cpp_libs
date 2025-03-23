#include "arith_utils.h"
#include "random.h"

int greatestCommonDivisor(int a, int b) {
    while (b != 0) {
        const int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int integerPartition(const int x) {
    const int n = x / 3;
    const int m = x % 3;

    switch (m) {
        case 0:
            return poweri(3, n);
        case 1:
            if (n == 1)
                return 1;
            return 4 * poweri(3, n - 1);
        default:
            return 2 * poweri(3, n);
    }
}

static uint64_t primalityTestHelper(const unsigned base, const unsigned exponent, const unsigned n) {
    if (exponent == 0)
        return 1;

    // 递归计算 base ^ exponent % n
    const uint64_t x = primalityTestHelper(base, exponent / 2, n);
    if (x == 0)
        return 0;

    const uint64_t y = (uint64_t) x * x % n;

    // 中间检测
    if (y == 1 && x != 1 && x != n - 1)
        return 0;

    if (exponent % 2 != 0)
        return base * y % n;

    return y;
}

int primalityTest(const unsigned n, const unsigned numTest) {
    if (n < 2)
        return 0;

    if (n <= 3)
        return 1;

    for (unsigned time = 0; time < numTest; time++)
        if (primalityTestHelper(randint(2, n - 1), n - 1, n) != 1)
            return 0;

    return 1;
}

static void primeFactorizationHelper(int n, int factors[], int *numFactors) {
    while (n % 2 == 0) {
        factors[(*numFactors)++] = 2;
        n >>= 1;
    }

    int a = 0;
    int b2 = n;
    int b = sqrti(n);
    do {
        if (b <= a + 1) {
            factors[(*numFactors)++] = n;
            return;
        }

        if (squarei(b) == b2) {
            primeFactorizationHelper(a + b, factors, numFactors);
            primeFactorizationHelper(b - a, factors, numFactors);
            return;
        }

        b2 += (a << 1) | 1;
        if (squarei(b + 1) <= b2)
            ++b;
        ++a;
    } while (1);
}

void primeFactorization(const int n, int factors[], int *numFactors) {
    *numFactors = 0;
    primeFactorizationHelper(n, factors, numFactors);
}
