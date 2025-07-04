#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <stdint.h>
#include <string.h>

struct Attribute_ {
    uint64_t hash[2];
    Attribute *next;
    void *vector;
};

static inline void strHash16(const char str[16], GraphId hash[2]) {
    hash[0] = hash[1] = 0;
    strncpy((char *)hash, str, 16);
}

#endif //ATTRIBUTE_H
