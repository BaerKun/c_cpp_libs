#include <long_long_long.h>
#include <stdio.h>
#include <stdlib.h>

int8_t lldComp(const uint8_t *a, const uint8_t *b, const uint32_t size) {
    const uint8_t *aptr = a + size, *bptr = b + size;

    while (aptr != a) {
        --aptr;
        --bptr;
        if (*aptr > *bptr)
            return 1;
        if (*aptr < *bptr)
            return -1;
    }
    return 0;
}

void lllAdd(uint8_t *output, const uint8_t *input1, const uint8_t *input2, const uint32_t size) {
    uint16_t carry = 0;
    uint8_t *outptr = output;
    const uint8_t *inptr1 = input1, *inptr2 = input2, *end = output + size;

    do {
        const uint16_t buffer = carry + *inptr1 + *inptr2;
        carry = buffer >> 8;
        *outptr = buffer;
        ++inptr1;
        ++inptr2;
    } while (++outptr != end);
}

void lllAdd32(uint8_t *output, const uint8_t *input1, const uint32_t input2, const uint32_t size) {
    uint64_t buffer = input2;
    uint8_t *outptr = output;
    const uint8_t *inptr = input1,
        *end = output + size;

    while (outptr != end && buffer != 0) {
        buffer += *inptr;
        *outptr = buffer;
        buffer >>= 8;
        ++inptr;
        ++outptr;
    }
}

void lllComplement(uint8_t *out, const uint8_t *in, const uint32_t size) {
    const uint8_t *inptr = in, *end = in + size;
    uint8_t *outptr = out;

    do {
        if (inptr == end)
            return;
        if (*inptr != 0)
            break;
        *outptr = 0;
        ++outptr;
        ++inptr;
    } while (1);

    *outptr = ~*inptr + 1;
    while (++inptr != end) {
        ++outptr;
        *outptr = ~*inptr;
    }
}

void lllSub(uint8_t *output, const uint8_t *minuend, const uint8_t *subtracted, const uint32_t size) {
    uint8_t *complement = malloc(size);
    lllComplement(complement, subtracted, size);
    lllAdd(output, minuend, complement, size);
    free(complement);
}

void lllMul(uint8_t *output, const uint8_t *input1, const uint8_t *input2, const uint32_t size) {
    uint32_t *temp = calloc(size, 4);

    for (int j, i = 0; i != size; ++i) {
        for (j = size - i - 1; j >= 0; --j) {
            temp[i + j] += (uint32_t) input1[i] * input2[j];
        }
    }

    uint8_t *outptr = output, *end = output + size;
    uint32_t *this = temp;
    do {
        uint32_t *next = this + 1;
        *outptr = *this;
        *next += *this >> 8;
        this = next;
    } while (++outptr != end);

    free(temp);
}

void lllMul32(uint8_t *output, const uint8_t *input1, const uint32_t input2, const uint32_t size) {
    uint8_t *outptr = output;
    const uint8_t *inptr = input1, *end = input1 + size;
    uint64_t tmp = 0;

    while (inptr != end) {
        tmp = (tmp >> 8) + (uint64_t) *inptr * input2;
        *outptr = tmp;
        ++inptr;
        ++outptr;
    }
}

const uint8_t *lllHigh(const uint8_t *a, const uint32_t size) {
    const uint8_t *ptr = a + size;

    while (--ptr != a && *ptr == 0);

    return ptr;
}

void lllDiv8(uint8_t *quotient, const uint8_t *dividend, const uint8_t divisor, uint8_t *remainder, const uint32_t size) {
    if (divisor == 0) {
        fprintf(stderr, "Division by zero!\n");
        return;
    }

    uint8_t *quot_ptr = quotient + size;
    const uint8_t *dividend_ptr = dividend + size;
    uint16_t dividend_tmp = 0;

    while (quot_ptr != quotient) {
        --quot_ptr;
        --dividend_ptr;
        dividend_tmp = dividend_tmp << 8 | *dividend_ptr;
        if(quotient != NULL)
            *quot_ptr = dividend_tmp / divisor;
        dividend_tmp = dividend_tmp % divisor;
    }

    if (remainder != NULL)
        *remainder = dividend_tmp;
}

void lllDiv(uint8_t *quotient, const uint8_t *dividend, const uint8_t *divisor, uint8_t *remainder, const uint32_t size) {
    const uint8_t *divisor_high = lllHigh(divisor, size);
    const uint32_t dividend_size = lllHigh(dividend, size) - dividend + 1;
    const uint32_t divisor_size = divisor_high - divisor + 1;

    if (divisor_size == 1) {
        if(remainder != NULL)
            lllSet(remainder, 0, size);
        if(quotient != NULL)
            lllSet(quotient + dividend_size, 0, size - dividend_size);
        lllDiv8(quotient, dividend, *divisor, remainder, dividend_size);
        return;
    }

    if(quotient != NULL)
        lllSet(quotient, 0, size);
    if (dividend_size < divisor_size)
        return;

    uint16_t dividend_16 = 0;
    const uint16_t divisor_16 = *divisor_high << 8 | *(divisor_high - 1);
    const uint32_t divisor_size_plus = divisor_size + 1;

    uint8_t *remainder_ = remainder == NULL ? malloc(size) : remainder;
    uint8_t *remainder_high = remainder_ + dividend_size - 1;

    uint8_t *divisor_complement = malloc(divisor_size_plus * 2);
    uint8_t *subtracted_buffer = divisor_complement + divisor_size_plus;

    lllMov(remainder_, dividend, size);
    lllComplement(divisor_complement, divisor, divisor_size_plus);

    for (uint8_t *ptr = quotient + dividend_size - divisor_size; ptr >= quotient; --ptr, --remainder_high) {
        dividend_16 = dividend_16 << 8 | *remainder_high;
        if (dividend_16 < *divisor_high)
            continue;

        uint8_t quotient_tmp = (dividend_16 << 8 | *(remainder_high - 1)) / divisor_16;
        if (quotient_tmp == 0)
            continue;

        uint8_t *remainder_low = remainder_high - divisor_size + 1;
        lllMul32(subtracted_buffer, divisor, quotient_tmp, divisor_size_plus);

        if (lldComp(subtracted_buffer, remainder_low, divisor_size_plus) == 1) {
            lllAdd(subtracted_buffer, subtracted_buffer, divisor_complement, divisor_size_plus);
            --quotient_tmp;
        }

        if(quotient != NULL)
            *ptr = quotient_tmp;

        lllComplement(subtracted_buffer, subtracted_buffer, divisor_size_plus);
        lllAdd(remainder_low, remainder_low, subtracted_buffer, divisor_size_plus);
        dividend_16 = *remainder_high;
    }

    free(divisor_complement);
    if (remainder == NULL)
        free(remainder_);
}

void lllPrint16(const uint8_t *a, const uint32_t size) {
    const uint8_t *ptr = lllHigh(a, size);
    do {
        printf("%02x", *ptr);
    } while (ptr-- != a);
}

void lllPrint10(const uint8_t *a, const uint32_t size) {
    uint8_t *buffer = malloc(size + (int)(size * 8 * 0.301029995664) + 1);
    uint8_t *stack_buffer = buffer + size;
    uint8_t *top = stack_buffer;

    lllMov(buffer, a, size);
    const uint8_t *high_ptr = lllHigh(buffer, size);
    do {
        lllDiv8(buffer, buffer, 10, top, size);
        ++top;
        if(*high_ptr == 0) {
            if(high_ptr == buffer)
                break;
            --high_ptr;
        }
    } while (1);

    do {
        --top;
        printf("%d", *top);
    }while (top != stack_buffer);

    free(buffer);
}

void lllInput10(uint8_t *a, const uint8_t size) {
    lllSet(a, 0, size);
    while (1){
        const int c = getchar();
        if(c < '0' || c > '9')
            break;
        lllMul32(a, a, 10, size);
        lllAdd32(a, a, c - '0', size);
    }
}