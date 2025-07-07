#include "private/attribute.h"
#include <stdlib.h>

void graphAttrDestroyList(Attribute *attr) {
  for (Attribute *next; attr; attr = next) {
    next = attr->next;
    free(attr->vector);
    free(attr);
  }
}
