#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "graph_detail.h"
#include <string.h>

static inline void strHash16(const char str[16], uint64_t hash[2]) {
  hash[0] = hash[1] = 0;
  strncpy((char *)hash, str, 16);
}

void graphAttrDestroyList(Attribute *attr);

#endif //ATTRIBUTE_H
