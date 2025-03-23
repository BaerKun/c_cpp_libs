#ifndef MATH_FUNC_H
#define MATH_FUNC_H

typedef enum {
    CONSTANT = -2, INTERMEDIATE = -1, VARIABLE = 0
} NumberType;

typedef enum {
    UNKNOWN, ADD, NEG, MUL, REC, POW, EXP, LOG, SIN, COS, TAN
} OperationType;

typedef struct ComputationNode_ ComputationNode, *ComputationNodePtr, MathFunction;

struct ComputationNode_ {
    float value, grad;
    NumberType type;
    OperationType operation;
    int shared;
    ComputationNodePtr *end;
    ComputationNodePtr operand[0];
};

/*
 * 用constant/variable/intermediate创建的节点必须手动调用mfDelete删除
 * 而用mfAdd/...创建的节点只需要对“树”的根调用，
 */

ComputationNode *constant(float value);

ComputationNode *variable(int index);

ComputationNode *intermediate(OperationType operation);

void mfDelete(ComputationNodePtr node);

void linkOperand(ComputationNodePtr node, ComputationNodePtr operand);

float calculate(const MathFunction *func);

/*
 * 同时返回calculate(func)的值
 * 微分值存储在每个节点的grad中
 */
float autoDiff(MathFunction *func);

MathFunction *derivative(const MathFunction *func);

void mfPrint(const MathFunction *func);

ComputationNodePtr mfAdd(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfNeg(ComputationNodePtr a);

ComputationNodePtr mfSub(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfMul(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfRec(ComputationNodePtr a);

ComputationNodePtr mfDiv(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfPow(ComputationNodePtr a, ComputationNodePtr b);

ComputationNodePtr mfExp(ComputationNodePtr a);

ComputationNodePtr mfLog(ComputationNodePtr a);

ComputationNodePtr mfSin(ComputationNodePtr a);

ComputationNodePtr mfCos(ComputationNodePtr a);

ComputationNodePtr mfTan(ComputationNodePtr a);

#endif //MATH_FUNC_H
