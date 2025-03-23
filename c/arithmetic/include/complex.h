#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>
#undef complex

typedef struct {
    double real;
    double imag;
} Complex;

static inline Complex complexAdd(Complex a, Complex b) {
    return (Complex) {a.real + b.real, a.imag + b.imag};
}

static inline Complex complexSub(Complex a, Complex b) {
    return (Complex) {a.real - b.real, a.imag - b.imag};
}

static inline Complex complexMul(Complex a, Complex b) {
    return (Complex) {a.real * b.real - a.imag * b.imag, a.real * b.imag + a.imag * b.real};
}

static inline Complex complexDiv(Complex a, Complex b) {
    const double denominator = b.real * b.real + b.imag * b.imag;
    return (Complex) {(a.real * b.real + a.imag * b.imag) / denominator, (a.imag * b.real - a.real * b.imag) / denominator};
}

static inline  Complex complexFromArgAndMod(double argument, double modulus) {
    return (Complex) {modulus * cos(argument), modulus * sin(argument)};
}

int Horner(const int polynomial[], int x, int size);

int CooleyTukey(const int polynomial[], int x, int size);

void dft(const double time[], Complex frequency[], int size);

void idft(const Complex frequency[], double time[], int size);

void dftConv(const double a[], const double b[], double c[], int size_a, int size_b);

#endif //COMPLEX_H
