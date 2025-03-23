#include "math_func.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


static ComputationNodePtr autoConstant(const float value) {
    const ComputationNodePtr node = malloc(sizeof(ComputationNode));
    node->type = CONSTANT;
    node->operation = UNKNOWN;
    node->value = value;
    node->shared = 0;
    return node;
}

static ComputationNodePtr autoIntermediate(const OperationType operation) {
    static int numberOperands[] = {0, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1};

    const ComputationNodePtr node = malloc(
        sizeof(ComputationNode) + numberOperands[operation] * sizeof(ComputationNodePtr));
    node->type = INTERMEDIATE;
    node->operation = operation;
    node->end = node->operand;
    node->shared = 0;
    return node;
}

ComputationNode *constant(const float value) {
    const ComputationNodePtr node = autoConstant(value);
    node->shared = 1;
    return node;
}

ComputationNode *variable(const int index) {
    const ComputationNodePtr node = malloc(sizeof(ComputationNode));
    node->type = index;
    node->operation = UNKNOWN;
    node->shared = 1;
    return node;
}

ComputationNode *intermediate(const OperationType operation) {
    const ComputationNodePtr node = autoIntermediate(operation);
    node->shared = 1;
    return node;
}

void linkOperand(const ComputationNodePtr node, const ComputationNodePtr operand) {
    if (node->type != INTERMEDIATE)
        return;

    *node->end = operand;
    node->end += 1;
    operand->shared += 1;
}


static void delete_(const ComputationNodePtr node) {
    if (node->type == INTERMEDIATE) {
        for (const ComputationNodePtr *op = node->operand; op != node->end; ++op) {
            const ComputationNodePtr operand = *op;
            if (--operand->shared == 0)
                delete_(operand);
        }
    }
    free(node);
}

void mfDelete(const ComputationNodePtr node) {
    if (--node->shared <= 0)
        delete_(node);
}

static inline void deleteAutoConstInter(const ComputationNodePtr node) {
    if (node->shared == 0)
        free(node);
}


static inline float square(const float x) {
    return x * x;
}

float calculate(const MathFunction *const func) {
    if (func->type != INTERMEDIATE)
        return func->value;

    const ComputationNodePtr *const operand = func->operand;
    switch (func->operation) {
        case ADD:
            return calculate(operand[0]) + calculate(operand[1]);
        case NEG:
            return -calculate(operand[0]);
        case MUL:
            return calculate(operand[0]) * calculate(operand[1]);
        case REC:
            return 1.f / calculate(operand[0]);
        case POW:
            return powf(calculate(operand[0]), calculate(operand[1]));
        case EXP:
            return expf(calculate(operand[0]));
        case LOG:
            return logf(calculate(operand[0]));
        case SIN:
            return sinf(calculate(operand[0]));
        case COS:
            return cosf(calculate(operand[0]));
        case TAN:
            return tanf(calculate(operand[0]));
        default:
            return 0.f;
    }
}

static float forward(MathFunction *const func) {
    func->grad = 0.f;
    if (func->type != INTERMEDIATE)
        return func->value;

    const ComputationNodePtr *const operand = func->operand;
    switch (func->operation) {
        case ADD:
            return func->value = forward(operand[0]) + forward(operand[1]);
        case NEG:
            return func->value = -forward(operand[0]);
        case MUL:
            return func->value = forward(operand[0]) * forward(operand[1]);
        case REC:
            return func->value = 1.f / forward(operand[0]);
        case POW:
            return func->value = powf(forward(operand[0]), forward(operand[1]));
        case EXP:
            return func->value = expf(forward(operand[0]));
        case LOG:
            return func->value = logf(forward(operand[0]));
        case SIN:
            return func->value = sinf(forward(operand[0]));
        case COS:
            return func->value = cosf(forward(operand[0]));
        case TAN:
            return func->value = tanf(forward(operand[0]));
        default:
            return 0.f;
    }
}

static void backward(const MathFunction *const func, float grad) {
    if (func->type != INTERMEDIATE)
        return;

    const ComputationNodePtr *const operand = func->operand;
    float _grad;
    switch (func->operation) {
        case ADD:
            operand[0]->grad += grad;
            operand[1]->grad += grad;
            backward(operand[0], grad);
            backward(operand[1], grad);
            return;
        case NEG:
            grad = -grad;
            operand[0]->grad += grad;
            backward(operand[0], grad);
            return;
        case MUL:
            _grad = grad * operand[1]->value;
            operand[0]->grad += _grad;
            backward(operand[0], _grad);

            _grad = grad * operand[0]->value;
            operand[1]->grad += _grad;
            backward(operand[1], _grad);
            return;
        case REC:
            grad = -grad / square(operand[0]->value);
            operand[0]->grad += grad;
            backward(operand[0], grad);
            return;
        case POW:
            grad *= func->value;

            _grad = grad * operand[1]->value / operand[0]->value;
            operand[0]->grad += _grad;
            backward(operand[0], _grad);

            _grad = grad * logf(operand[0]->value);
            operand[1]->grad += _grad;
            backward(operand[1], _grad);
            return;
        case EXP:
            grad *= func->value;
            operand[0]->grad += grad;
            backward(operand[0], grad);
            return;
        case LOG:
            grad /= operand[0]->value;
            operand[0]->grad += grad;
            backward(operand[0], grad);
            return;
        case SIN:
            grad *= cosf(operand[0]->value);
            operand[0]->grad += grad;
            backward(operand[0], grad);
            return;
        case COS:
            grad *= -sinf(operand[0]->value);
            operand[0]->grad += grad;
            backward(operand[0], grad);
            return;
        case TAN:
            grad *= 1.f + square(func->value);
            operand[0]->grad += grad;
            backward(operand[0], grad);
        default:
            break;
    }
}

float autoDiff(MathFunction *const func) {
    const float result = forward(func);
    backward(func, func->grad = 1.f);
    return result;
}

MathFunction *derivative(const MathFunction *const func) {
    if (func->type == CONSTANT)
        return autoConstant(0);
    if (func->type == VARIABLE)
        return autoConstant(1);

    const ComputationNodePtr *const operand = func->operand;
    switch (func->operation) {
        case ADD:
            return mfAdd(derivative(operand[0]), derivative(operand[1]));
        case NEG:
            return mfNeg(derivative(operand[0]));
        case MUL:
            return mfAdd(mfMul(derivative(operand[0]), operand[1]), mfMul(operand[0], derivative(operand[1])));
        case REC:
            return mfNeg(mfMul(derivative(operand[0]), mfPow(operand[0], autoConstant(-2))));
        case POW:
            return mfAdd(mfMul(mfMul(derivative(operand[0]), operand[1]),
                               mfPow(operand[0], mfAdd(operand[1], autoConstant(-1)))),
                         mfMul(mfMul(derivative(operand[1]), mfLog(operand[0])), mfPow(operand[0], operand[1])));
        case EXP:
            return mfMul(derivative(operand[0]), mfExp(operand[0]));
        case LOG:
            return mfMul(derivative(operand[0]), operand[0]);
        case SIN:
            return mfMul(derivative(operand[0]), mfCos(operand[0]));
        case COS:
            return mfNeg(mfMul(derivative(operand[0]), mfSin(operand[0])));
        case TAN:
            return mfMul(derivative(operand[0]), mfPow(mfCos(operand[0]), autoConstant(-2)));
        default:
            return NULL;
    }
}


void mfPrint(const MathFunction *const func) {
    if (func->type == CONSTANT) {
        printf("%.2f", func->value);
        return;
    }
    if (func->type == VARIABLE) {
        printf("x%d", func->type);
        return;
    }
    const ComputationNodePtr *const operand = func->operand;
    switch (func->operation) {
        case ADD:
            putchar('(');
            mfPrint(operand[0]);
            printf(" + ");
            mfPrint(operand[1]);
            putchar(')');
            return;
        case MUL:
            mfPrint(operand[0]);
            printf(" * ");
            mfPrint(operand[1]);
            return;
        case NEG:
            putchar('-');
            mfPrint(operand[0]);
            return;
        case REC:
            putchar('(');
            mfPrint(operand[0]);
            printf(")^(-1)");
            return;
        case EXP:
            printf("exp(");
            mfPrint(operand[0]);
            putchar(')');
            return;
        case LOG:
            printf("log(");
            mfPrint(operand[0]);
            putchar(')');
            return;
        case POW:
            putchar('(');
            mfPrint(operand[0]);
            printf(")^(");
            mfPrint(operand[1]);
            putchar(')');
            return;
        case SIN:
            printf("sin(");
            mfPrint(operand[0]);
            putchar(')');
            return;
        case COS:
            printf("cos(");
            mfPrint(operand[0]);
            putchar(')');
            return;
        case TAN:
            printf("tan(");
            mfPrint(operand[0]);
            putchar(')');
        default:
            break;
    }
}


static inline ComputationNodePtr add_(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = autoIntermediate(ADD);
    linkOperand(node, a);
    linkOperand(node, b);
    return node;
}

ComputationNodePtr mfAdd(const ComputationNodePtr a, const ComputationNodePtr b) {
    switch ((b->type == CONSTANT) << 1 | (a->type == CONSTANT)) {
        case 0:
            return add_(a, b);
        case 1:
            if (a->value != 0.f)
                return add_(a, b);
            deleteAutoConstInter(a);
            return b;
        case 2:
            if (b->value != 0.f)
                return add_(a, b);
            deleteAutoConstInter(b);
            return a;
        default: {
            const float value = a->value + b->value;
            deleteAutoConstInter(a);
            deleteAutoConstInter(b);
            return autoConstant(value);
        }
    }
}

ComputationNodePtr mfNeg(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = -a->value;
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(NEG);
    linkOperand(node, a);
    return node;
}

ComputationNodePtr mfSub(const ComputationNodePtr a, const ComputationNodePtr b) {
    return mfAdd(a, mfNeg(b));
}

static inline ComputationNodePtr mul_(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = autoIntermediate(MUL);
    linkOperand(node, a);
    linkOperand(node, b);
    return node;
}

ComputationNodePtr mfMul(const ComputationNodePtr a, const ComputationNodePtr b) {
    switch ((b->type == CONSTANT) << 1 | (a->type == CONSTANT)) {
        case 0:
            return mul_(a, b);
        case 1:
            if (a->value == 0.f) {
                deleteAutoConstInter(b);
                return a;
            }
            if (a->value == 1.f) {
                deleteAutoConstInter(a);
                return b;
            }
            return mul_(a, b);
        case 2:
            if (b->value == 0.f) {
                deleteAutoConstInter(a);
                return b;
            }
            if (b->value == 1.f) {
                deleteAutoConstInter(b);
                return a;
            }
            return mul_(a, b);
        default: {
            const float value = a->value * b->value;
            deleteAutoConstInter(a);
            deleteAutoConstInter(b);
            return autoConstant(value);
        }
    }
}

ComputationNodePtr mfRec(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = 1.f / a->value;
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(REC);
    linkOperand(node, a);
    return node;
}

ComputationNodePtr mfDiv(const ComputationNodePtr a, const ComputationNodePtr b) {
    return mfMul(a, mfRec(b));
}

static inline ComputationNodePtr pow_(const ComputationNodePtr a, const ComputationNodePtr b) {
    const ComputationNodePtr node = autoIntermediate(POW);
    linkOperand(node, a);
    linkOperand(node, b);
    return node;
}

ComputationNodePtr mfPow(const ComputationNodePtr a, const ComputationNodePtr b) {
    switch ((b->type == CONSTANT) << 1 | a->type == CONSTANT) {
        case 0:
            return pow_(a, b);
        case 1:
            if (a->value == 0.f || a->value == 1.f) {
                deleteAutoConstInter(b);
                return a;
            }
            return pow_(a, b);
        case 2:
            if (b->value == 0.f) {
                deleteAutoConstInter(a);
                deleteAutoConstInter(b);
                return autoConstant(1.f);
            }
            if (b->value == 1.f) {
                deleteAutoConstInter(b);
                return a;
            }
            return pow_(a, b);
        default:
            const float value = powf(a->value, b->value);
            deleteAutoConstInter(a);
            deleteAutoConstInter(b);
            return autoConstant(value);
    }
}

ComputationNodePtr mfExp(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = expf(a->value);
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(EXP);
    linkOperand(node, a);
    return node;
}

ComputationNodePtr mfLog(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = logf(a->value);
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(LOG);
    linkOperand(node, a);
    return node;
}

ComputationNodePtr mfSin(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = sinf(a->value);
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(SIN);
    linkOperand(node, a);
    return node;
}

ComputationNodePtr mfCos(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = cosf(a->value);
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(COS);
    linkOperand(node, a);
    return node;
}

ComputationNodePtr mfTan(const ComputationNodePtr a) {
    if (a->type == CONSTANT) {
        const float value = tanf(a->value);
        deleteAutoConstInter(a);
        return autoConstant(value);
    }

    const ComputationNodePtr node = autoIntermediate(TAN);
    linkOperand(node, a);
    return node;
}