#include <stdio.h>

#define calculate(f, b, o) { \
    array[j] = num##f o num##b; \
    if (calculate24(array, len)) { \
        printf("%d " #o " %d = %d\n", num##f, num##b, array[j]); \
        array[j] = numj;    \
        array[i] = numi;    \
        return 1;           \
    }                       \
}

int calculate24(int array[], int len) {
    if (--len == 0 && array[0] == 24)
        return 1;
    int i, j, numi, numj;
    for (i = 0; i < len; i++) {
        numi = array[i];
        array[i] = array[len];
        for (j = i; j < len; j++) {
            numj = array[j];
            calculate(i, j, +)
            calculate(i, j, *)
            if (numi > numj) {
                calculate(i, j, -)
                if (numj != 0 && numi % numj == 0) calculate(i, j, /)
            } else {
                calculate(j, i, -)
                if (numi != 0 && numj % numi == 0) calculate(j, i, /)
            }
            array[j] = numj;
        }
        array[i] = numi;
    }
    return 0;
}

void Calculate24(int array[], int len) {
    if (!calculate24(array, len))
        puts("No Solution\n");
}

int main() {
    int n;
    printf("数字的个数：");
    scanf("%d", &n);
    int array[n];
    printf("输入数字：");
    for (int i = 0; i < n; i++)
        scanf("%d", array + i);
    Calculate24(array, n);
    return 0;
}
