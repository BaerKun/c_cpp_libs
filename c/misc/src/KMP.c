#include "link_node.h"
#include <stdio.h>

// Partial Match Table, PMT
void calcPMT(const char *pattern, int *pmt) {
  pmt[0] = 0;
  int i = 1, l = 0;
  while (pattern[i]) {
    if (pattern[i] == pattern[l])
      pmt[i++] = ++l;
    else if (l != 0)
      l = pmt[l - 1];
    else
      pmt[i++] = 0;
  }
}

void KMP(const char *text, const char *pattern, const int *pmt,
         LinkNode **result) {
  int i = 0, j = 0;
  LinkNode *node = NULL;

  while (text[i]) {
    if (text[i] == pattern[j]) {
      ++i;
      if (!pattern[++j]) {
        LinkNode *next = newNode(i - j);
        if (node == NULL)
          *result = next;
        else
          node->next = next;
        node = next;

        j = pmt[j - 1];
      }
    } else if (j != 0)
      j = pmt[j - 1];
    else
      ++i;
  }

  if (node)
    node->next = NULL;
  else
    *result = NULL;
}