#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

static void disjointSetInit(int set[], const int number) {
    for (int i = 0; i < number; i++)
        set[i] = -1;
}

static void disjointSetUnion(int set[], const int class1, const int class2) {
    int *neg_height = set;
    if (neg_height[class1] > neg_height[class2])
        set[class1] = class2;
    else {
        if (neg_height[class1] == neg_height[class2])
            neg_height[class1]--;
        set[class2] = class1;
    }
}

static int disjointSetFind(int set[], const int id) {
    int cls;
    for (cls = id; set[cls] >= 0; cls = set[cls]) {
    }
    for (int i = id; i != cls;) {
        const int next = set[i];
        set[i] = cls;
        i = next;
    }
    return cls;
}

#endif //DISJOINT_SET_H
