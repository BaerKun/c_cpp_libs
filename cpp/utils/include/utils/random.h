#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

struct pcg_state_setseq_64 {
    uint64_t state;
    uint64_t inc;
};

typedef struct pcg_state_setseq_64 pcg32_random_t;

#define PCG32_INITIALIZER   { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

static uint32_t pcg32_random(void) {
    const uint64_t oldstate = pcg32_global.state;
    pcg32_global.state = oldstate * 6364136223846793005ULL + pcg32_global.inc;
    const uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    const uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static inline void pcg32_srandom(const uint64_t initstate, const uint64_t initseq) {
    pcg32_global.state = 0U;
    pcg32_global.inc = (initseq << 1u) | 1u;
    pcg32_random();
    pcg32_global.state += initstate;
    pcg32_random();
}

// return a <= x < b
static inline int32_t randint(const int32_t a, const int32_t b) {
    return a + pcg32_random() % (b - a);
}

static inline int64_t randlong(const int64_t a, const int64_t b) {
    return a + ((uint64_t)pcg32_random() << 32 | pcg32_random()) % (b - a);
}

static inline float randfloat(const float a, const float b) {
    return a + (b - a) * (float) pcg32_random() / 4294967296.f;
}

static inline double randdouble(const double a, const double b) {
    return a + (b - a) * pcg32_random() / 4294967296.0;
}

static void shuffleArray(int *array, const int size) {
    for (int i = 1, j, c; i < size; ++i) {
        j = randint(0, i + 1);
        c = array[i];
        array[i] = array[j];
        array[j] = c;
    }
}

#endif //RANDOM_H
