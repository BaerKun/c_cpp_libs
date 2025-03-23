#include <stdio.h>
#include <string.h>
#include "list.h"

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

void KMP(const char *text, const char *pattern, const int *pmt, ListPtr *result) {
    int i = 0, j = 0;
    NodePtr node = NULL, next;

    while (text[i]) {
        if (text[i] == pattern[j]) {
            ++i;
            if (!pattern[++j]) {
                next = newNode(i - j);
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

int main() {
    ListPtr result;
    char text[] = "https://www.google.com/search?q=%E7%BF%BB%E8%AF%91&oq=&gs_lcrp=EgZjaHJvbWUqCQgAECMYJxjqAjIJCAAQIxgnGOoCMgkIARAjGCcY6gIyCQgCECMYJxjqAjIJCAMQIxgnGOoCMgkIBBAjGCcY6gIyCQgFECMYJxjqAjIJCAYQIxgnGOoCMgkIBxAjGCcY6gLSAQkxMDEzajBqMTWoAgiwAgE&sourceid=chrome&ie=UTF-8";
    char pattern[] = "www.google.com";
    int pmt[sizeof(pattern) - 1];
    calcPMT(pattern, pmt);
    KMP(text, pattern, pmt, &result);
    for (NodePtr node = result; node; node = node->next) {
        printf("%d ", node->element);
    }
    list_destroy(result);
    return 1;
}
