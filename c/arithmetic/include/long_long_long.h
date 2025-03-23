#ifndef LONG_LONG_LONG_H
#define LONG_LONG_LONG_H

#include <stdint.h>
#include <string.h>

static inline void lllMov(uint8_t *a, const uint8_t *b, const uint32_t size){
    memcpy(a, b, size);
}

static inline void lllSet(uint8_t *a, const uint8_t val, const uint32_t size) {
    memset(a, val, size);
}

// a > b -> 1; a == b -> 0; a < b -> -1
int8_t lldComp(const uint8_t *a, const uint8_t *b, uint32_t size);

void lllAdd(uint8_t *output, const uint8_t *input1, const uint8_t *input2, uint32_t size);

void lllAdd32(uint8_t *output, const uint8_t *input1, uint32_t input2, uint32_t size);

void lllComplement(uint8_t *out, const uint8_t *in, uint32_t size);

void lllSub(uint8_t *output, const uint8_t *minuend, const uint8_t *subtracted, uint32_t size);

void lllMul(uint8_t *output, const uint8_t *input1, const uint8_t *input2, uint32_t size);

void lllMul32(uint8_t *output, const uint8_t *input1, uint32_t input2, uint32_t size);

const uint8_t *lllHigh(const uint8_t *a, uint32_t size);

void lllDiv8(uint8_t *quotient, const uint8_t *dividend, uint8_t divisor, uint8_t *remainder, uint32_t size);

void lllDiv(uint8_t *quotient, const uint8_t *dividend, const uint8_t *divisor, uint8_t *remainder, uint32_t size);

void lllPrint16(const uint8_t *a, uint32_t size);

void lllPrint10(const uint8_t *a, uint32_t size);

void lllInput10(uint8_t *a, uint8_t size);

#endif //LONG_LONG_LONG_H
