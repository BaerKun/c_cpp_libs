#ifndef COMPLEX_H
#define COMPLEX_H

#include <math.h>
#undef complex

typedef struct {
  double real;
  double imag;
} Complex;

static inline Complex complexAdd(const Complex a, const Complex b) {
  return (Complex){a.real + b.real, a.imag + b.imag};
}

static inline Complex complexSub(const Complex a, const Complex b) {
  return (Complex){a.real - b.real, a.imag - b.imag};
}

static inline Complex complexMul(const Complex a, const Complex b) {
  return (Complex){a.real * b.real - a.imag * b.imag,
                   a.real * b.imag + a.imag * b.real};
}

static inline Complex complexDiv(const Complex a, const Complex b) {
  const double denominator = b.real * b.real + b.imag * b.imag;
  return (Complex){(a.real * b.real + a.imag * b.imag) / denominator,
                   (a.imag * b.real - a.real * b.imag) / denominator};
}

static inline Complex complexFromArgAndMod(const double argument,
                                           const double modulus) {
  return (Complex){modulus * cos(argument), modulus * sin(argument)};
}

int Horner(const int polynomial[], int x, int size);

int CooleyTukey(const int polynomial[], int x, int size);

void dft(const double time[], Complex frequency[], int size);

void idft(const Complex frequency[], double time[], int size);

void dftConv(const double a[], const double b[], double c[], int size_a,
             int size_b);

#endif  // COMPLEX_H
