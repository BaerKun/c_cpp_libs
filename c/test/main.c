#include "math_func.h"
#include <stdio.h>

int main() {
    const ComputationNodePtr x = variable(0);
    MathFunction *func = mfAdd(x, mfDiv(x, constant(2)));
    MathFunction *deriv = derivative(func);

    mfPrint(func);
    putchar('\n');
    mfPrint(deriv);

    mfDelete(func);
    mfDelete(deriv);
    return 0;
}