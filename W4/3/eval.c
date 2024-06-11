#include "eval.h"
#include "Stack.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void calcNumStack(struct Stack *numStack, char op);

double RPNCalc(char *s) {
    RPNRegular(s);
    struct Stack *numStack = Stack_createStack(0U);

    size_t len = strlen(s);
    double currentNum = 0.0;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(s[i])) {
            currentNum = 10 * currentNum + s[i] - '0';
        } else if (s[i] == ' ') {
            struct Stack_Data d;
            d.data.number = currentNum;
            Stack_push(numStack, &d);
            currentNum = 0.0;
        } else if (isOp(s[i])) {
            calcNumStack(numStack, s[i]);
        }
    }

    double result = Stack_pop(numStack)->data.number;
    Stack_destroyStack(numStack);
    return result;
}
void infixToRPN(char *infix, char *target) {
    infixRegular(infix);
    size_t index = 0;
    size_t infixLen = strlen(infix);
    struct Stack *opStack = Stack_createStack(0U);
    bool toPrintSpace = false;
    for (size_t i = 0; i < infixLen; i++) {
        if (isdigit(infix[i])) {
            target[index] = infix[i];
            index++;
            toPrintSpace = true;
        } else {
            if (toPrintSpace) {
                target[index] = ' ';
                index++;

                toPrintSpace = false;
            }

            if (isOp(infix[i])) {
                while (!Stack_isEmpty(opStack)) {
                    struct Stack_Data *top = Stack_getTop(opStack);
                    if ((top->data.op != TOP_OP_OPEN) &&
                        (opCmp(top->data.op, infix[i]) >= 0)) {
                        struct Stack_Data *ret = Stack_pop(opStack);
                        target[index] = ret->data.op;
                        index++;
                        Stack_destroyData(ret);
                    } else {
                        break;
                    }
                }
                struct Stack_Data current;
                current.data.op = infix[i];
                Stack_push(opStack, &current);
            } else if (infix[i] == TOP_OP_OPEN) {
                struct Stack_Data current;
                current.data.op = infix[i];
                Stack_push(opStack, &current);
            } else if (infix[i] == TOP_OP_CLOSE) {
                while (!Stack_isEmpty(opStack)) {
                    struct Stack_Data *ret = Stack_pop(opStack);

                    if (ret->data.op != TOP_OP_OPEN) {
                        target[index] = ret->data.op;
                        index++;
                    } else {
                        Stack_destroyData(ret);
                        break;
                    }

                    Stack_destroyData(ret);
                }
            }
        }
    }

    while (!Stack_isEmpty(opStack)) {
        struct Stack_Data *ret = Stack_pop(opStack);
        target[index] = ret->data.op;
        index++;
        Stack_destroyData(ret);
    }

    target[index] = '\0';
    Stack_destroyStack(opStack);
}
double eval(char *infix) {
    infixRegular(infix);
    struct Stack *numStack = Stack_createStack(0U);
    struct Stack *opStack = Stack_createStack(0U);

    size_t len = strlen(infix);
    double currentNum = 0.0;
    bool pushed = false;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(infix[i])) {
            currentNum = 10 * currentNum + infix[i] - '0';
            pushed = false;
        } else {
            if (pushed == false) {
                struct Stack_Data d;
                d.data.number = currentNum;
                Stack_push(numStack, &d);
                pushed = true;
                currentNum = 0.0;
            }

            if (isOp(infix[i])) {
                while (!Stack_isEmpty(opStack)) {
                    struct Stack_Data *top = Stack_getTop(opStack);
                    if ((top->data.op != TOP_OP_OPEN) &&
                        (opCmp(top->data.op, infix[i]) >= 0)) {
                        struct Stack_Data *ret = Stack_pop(opStack);

                        calcNumStack(numStack, ret->data.op);

                        Stack_destroyData(ret);
                    } else {
                        break;
                    }
                }
                struct Stack_Data current;
                current.data.op = infix[i];
                Stack_push(opStack, &current);
            } else if (infix[i] == TOP_OP_OPEN) {
                struct Stack_Data current;
                current.data.op = infix[i];
                Stack_push(opStack, &current);
            } else if (infix[i] == TOP_OP_CLOSE) {
                while (!Stack_isEmpty(opStack)) {
                    struct Stack_Data *ret = Stack_pop(opStack);

                    if (ret->data.op != TOP_OP_OPEN) {
                        calcNumStack(numStack, ret->data.op);
                        Stack_destroyData(ret);
                    } else {
                        Stack_destroyData(ret);
                        break;
                    }
                }
            }
        }
    }
    if (pushed == false) {
        struct Stack_Data d;
        d.data.number = currentNum;
        Stack_push(numStack, &d);
        pushed = true;
        currentNum = 0.0;
    }

    while (!Stack_isEmpty(opStack)) {
        struct Stack_Data *ret = Stack_pop(opStack);
        calcNumStack(numStack, ret->data.op);
        Stack_destroyData(ret);
    }

    struct Stack_Data *result = Stack_pop(numStack);
    double r = result->data.number;

    Stack_destroyData(result);
    Stack_destroyStack(numStack);
    Stack_destroyStack(opStack);

    return r;
}

void calcNumStack(struct Stack *numStack, char op) {
    double a = 0.0;
    double b = 0.0;

    struct Stack_Data *numRet = Stack_pop(numStack);
    b = numRet->data.number;
    Stack_destroyData(numRet);
    numRet = Stack_pop(numStack);
    a = numRet->data.number;
    Stack_destroyData(numRet);

    double r = applyOp(op, a, b);

    struct Stack_Data result;
    result.data.number = r;
    Stack_push(numStack, &result);
}

void Stack_destroyData(struct Stack_Data *target) { free(target); }
