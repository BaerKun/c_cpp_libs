#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>

#ifdef __unix
#include <time.h>

typedef struct {
    unsigned long long read;
    struct timespec last;
} Timer;

static inline void timerInit(Timer *timer) {
    timer->read = 0llu;
}

static inline void timerStart(Timer *timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->last);
}

static inline void timerStop(Timer *timer) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    timer->read += (now.tv_sec - timer->last.tv_sec) * 1000000000llu + (now.tv_nsec - timer->last.tv_nsec);
}

static inline unsigned long long timerGetns(const Timer *timer) {
    return timer->read;
}
#endif

#ifdef _WIN32
// Windows下的Timer精度只有100ns
#include <windows.h>

typedef struct {
    LARGE_INTEGER freq;
    LARGE_INTEGER read;
    LARGE_INTEGER last;
} Timer;

static inline void timerInit(Timer *timer) {
    QueryPerformanceFrequency(&timer->freq);
    timer->read.QuadPart = 0;
}

static inline void timerStart(Timer *timer) {
    QueryPerformanceCounter(&timer->last);
}

static inline void timerStop(Timer *timer) {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    timer->read.QuadPart += now.QuadPart - timer->last.QuadPart;
}

static inline unsigned long long timerGetns(const Timer *timer) {
    return timer->read.QuadPart * 1000000000llu / timer->freq.QuadPart;
}

#endif

static Timer __static_timer__;

#define TEST_TIME(code) \
    timerInit(&__static_timer__); \
    timerStart(&__static_timer__); \
    code; \
    timerStop(&__static_timer__); \
    printf("time: %llu ns\n", timerGetns(&__static_timer__))

#endif //TIMER_H
