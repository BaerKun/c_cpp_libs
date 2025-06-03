#include "math_func.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

ComputationNodePtr autoConstant(const float value) {
  const ComputationNodePtr node = malloc(sizeof(ComputationNode));
  node->type = CONSTANT;
  node->operation = UNKNOWN;
  node->value = value;
  node->shared = 0;
  return node;
}

ComputationNodePtr autoIntermediate(const OperationType operation) {
  static int numberOperands[] = {0, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1};

  const ComputationNodePtr node =
      malloc(sizeof(ComputationNode) +
             numberOperands[operation] * sizeof(ComputationNodePtr));
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

void linkOperand(const ComputationNodePtr node,
                 const ComputationNodePtr operand) {
  if (node->type != INTERMEDIATE) return;

  *node->end = operand;
  node->end += 1;
  operand->shared += 1;
}

static void delete_(const ComputationNodePtr node) {
  if (node->type == INTERMEDIATE) {
    for (const ComputationNodePtr *op = node->operand; op != node->end; ++op) {
      const ComputationNodePtr operand = *op;
      if (--operand->shared == 0) delete_(operand);
    }
  }
  free(node);
}

void mfDelete(const ComputationNodePtr node) {
  if (--node->shared <= 0) delete_(node);
}

static inline float square(const float x) { return x * x; }

float calculate(const MathFunction *const func) {
  if (func->type != INTERMEDIATE) return func->value;

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
  if (func->type != INTERMEDIATE) return func->value;

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
  if (func->type != INTERMEDIATE) return;

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
  if (func->type == CONSTANT) return autoConstant(0);
  if (func->type == VARIABLE) return autoConstant(1);

  const ComputationNodePtr *const operand = func->operand;
  switch (func->operation) {
  case ADD:
    return mfAdd(derivative(operand[0]), derivative(operand[1]));
  case NEG:
    return mfNeg(derivative(operand[0]));
  case MUL:
    return mfAdd(mfMul(derivative(operand[0]), operand[1]),
                 mfMul(operand[0], derivative(operand[1])));
  case REC:
    return mfNeg(
        mfMul(derivative(operand[0]), mfPow(operand[0], autoConstant(-2))));
  case POW:
    return mfAdd(mfMul(mfMul(derivative(operand[0]), operand[1]),
                       mfPow(operand[0], mfAdd(operand[1], autoConstant(-1)))),
                 mfMul(mfMul(derivative(operand[1]), mfLog(operand[0])),
                       mfPow(operand[0], operand[1])));
  case EXP:
    return mfMul(derivative(operand[0]), mfExp(operand[0]));
  case LOG:
    return mfMul(derivative(operand[0]), operand[0]);
  case SIN:
    return mfMul(derivative(operand[0]), mfCos(operand[0]));
  case COS:
    return mfNeg(mfMul(derivative(operand[0]), mfSin(operand[0])));
  case TAN:
    return mfMul(derivative(operand[0]),
                 mfPow(mfCos(operand[0]), autoConstant(-2)));
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
