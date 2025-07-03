#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <stdint.h>
#include <stdlib.h>
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

static void destroyAttributeList(Attribute *attr) {
    for (Attribute *next; attr; attr = next) {
        next = attr->next;
        free(attr->vector);
        free(attr);
    }
}

#endif //ATTRIBUTE_H
