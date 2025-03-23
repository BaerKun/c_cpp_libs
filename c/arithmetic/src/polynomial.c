#include <stdlib.h>
#include "complex.h"
#include <string.h>

int Horner(const int polynomial[], const int x, const int size) {
    int y = 0;
    for (const int *a = polynomial + size - 1; a >= polynomial; --a)
        y = y * x + *a;
    return y;
}

static int CooleyTukeyHelper(const int polynomial[], const int x, const int start, const int end, const int step) {
    if (start + step >= end)
        return polynomial[start];

    return CooleyTukeyHelper(polynomial, x * x, start, end, step << 1) +
           CooleyTukeyHelper(polynomial, x * x, start + step, end, step << 1) * x;
};

int CooleyTukey(const int polynomial[], const int x, const int size) {
    return CooleyTukeyHelper(polynomial, x, 0, size, 1);
}

static Complex dftHelper(const double time[], const Complex rotationFactor, const int start, const int end,
                         const int step) {
    if (start + step >= end)
        return (Complex){time[start], .0};

    const Complex rotationFactorSq = complexMul(rotationFactor, rotationFactor);

    return complexAdd(dftHelper(time, rotationFactorSq, start, end, step << 1),
                      complexMul(dftHelper(time, rotationFactorSq, start + step, end, step << 1), rotationFactor));
}

void dft(const double time[], Complex frequency[], const int size) {
    for (int i = 0; i < size; ++i)
        frequency[i] = dftHelper(time, complexFromArgAndMod(-2 * M_PI * i / size,
                                                            1.), 0, size, 1);
}

static Complex idftHelper(const Complex frequency[], const Complex rotationFactor, const int start, const int end,
                          const int step) {
    if (start + step >= end)
        return frequency[start];

    const Complex rotationFactorSq = complexMul(rotationFactor, rotationFactor);

    return complexAdd(idftHelper(frequency, rotationFactorSq, start, end, step << 1),
                      complexMul(idftHelper(frequency, rotationFactorSq, start + step, end, step << 1),
                                 rotationFactor));
}

void idft(const Complex frequency[], double time[], const int size) {
    for (int i = 0; i < size; ++i)
        time[i] = idftHelper(frequency, complexFromArgAndMod(2 * M_PI * i / size,
                                                             1.), 0, size, 1).real / size;
}

static void dftConvHelper(const double a[], const double b[], double c[], const int size) {
    Complex *memory = malloc(sizeof(Complex) * 3 * size);
    Complex *complexA = memory;
    Complex *complexB = memory + size;
    Complex *complexC = memory + 2 * size;
    dft(a, complexA, size);
    dft(b, complexB, size);

    for (int i = 0; i < size; ++i)
        complexC[i] = complexMul(complexA[i], complexB[i]);

    idft(complexC, c, size);
    free(memory);
}

void dftConv(const double a[], const double b[], double c[], const int size_a, const int size_b) {
    const int size = size_a + size_b - 1;
    double *memory = calloc(2 * size, sizeof(double));

    memcpy(memory, a, size_a * sizeof(double));
    memcpy(memory + size, b, size_b * sizeof(double));
    dftConvHelper(memory, memory + size, c, size);

    free(memory);
}
