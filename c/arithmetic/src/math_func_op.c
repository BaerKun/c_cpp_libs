#include "math_func.h"
#include <math.h>
#include <stdlib.h>

static inline void deleteAutoConstInter(const ComputationNodePtr node) {
  if (node->shared == 0) free(node);
}

static inline ComputationNodePtr add_(const ComputationNodePtr a,
                                      const ComputationNodePtr b) {
  const ComputationNodePtr node = autoIntermediate(ADD);
  linkOperand(node, a);
  linkOperand(node, b);
  return node;
}

ComputationNodePtr mfAdd(const ComputationNodePtr a,
                         const ComputationNodePtr b) {
  switch ((b->type == CONSTANT) << 1 | (a->type == CONSTANT)) {
  case 0:
    return add_(a, b);
  case 1:
    if (a->value != 0.f) return add_(a, b);
    deleteAutoConstInter(a);
    return b;
  case 2:
    if (b->value != 0.f) return add_(a, b);
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

ComputationNodePtr mfSub(const ComputationNodePtr a,
                         const ComputationNodePtr b) {
  return mfAdd(a, mfNeg(b));
}

static inline ComputationNodePtr mul_(const ComputationNodePtr a,
                                      const ComputationNodePtr b) {
  const ComputationNodePtr node = autoIntermediate(MUL);
  linkOperand(node, a);
  linkOperand(node, b);
  return node;
}

ComputationNodePtr mfMul(const ComputationNodePtr a,
                         const ComputationNodePtr b) {
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

ComputationNodePtr mfDiv(const ComputationNodePtr a,
                         const ComputationNodePtr b) {
  return mfMul(a, mfRec(b));
}

static inline ComputationNodePtr pow_(const ComputationNodePtr a,
                                      const ComputationNodePtr b) {
  const ComputationNodePtr node = autoIntermediate(POW);
  linkOperand(node, a);
  linkOperand(node, b);
  return node;
}

ComputationNodePtr mfPow(const ComputationNodePtr a,
                         const ComputationNodePtr b) {
  float value;
  switch ((b->type == CONSTANT) << 1 | (a->type == CONSTANT)) {
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
    value = powf(a->value, b->value);
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