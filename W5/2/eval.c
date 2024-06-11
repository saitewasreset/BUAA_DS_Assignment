#include "eval.h"
#include "Stack.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void calcNumStack(struct Stack *numStack, char op);

long long RPNCalc(const char *rpn) {
    size_t rpnLen = strlen(rpn);
    char *s = (char *)malloc((rpnLen + 1) * sizeof(char));
    strcpy(s, rpn);
    RPNRegular(s);
    struct Stack *numStack = Stack_createStack(0U);

    size_t len = strlen(s);
    long long currentNum = 0;
    bool pushed = false;
    for (size_t i = 0; i < len; i++) {
        if (isdigit(s[i])) {
            currentNum = 10 * currentNum + s[i] - '0';
            pushed = false;
        } else if (s[i] == ' ') {
            struct Stack_Data d;
            d.data.number = currentNum;
            Stack_push(numStack, &d);
            currentNum = 0;
            pushed = true;
        } else if (isOp(s[i])) {
            if (!pushed) {
                struct Stack_Data d;
                d.data.number = currentNum;
                Stack_push(numStack, &d);
                currentNum = 0;
                pushed = true;
            }
            if (numStack->currentSize >= 2) {
                calcNumStack(numStack, s[i]);
            }
        }
    }
    if (!pushed) {
        struct Stack_Data d;
        d.data.number = currentNum;
        Stack_push(numStack, &d);
        currentNum = 0;
    }
    struct Stack_Data *ret = Stack_pop(numStack);
    long long result = ret->data.number;
    free(s);
    Stack_destroyData(ret);
    Stack_destroyStack(numStack);
    return result;
}
void infixToRPN(const char *_infix, char *target) {
    char *infix = (char *)malloc((strlen(_infix) + 1) * sizeof(char));
    strcpy(infix, _infix);
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
    free(infix);
    Stack_destroyStack(opStack);
}
long long eval(const char *_infix) {
    char *infix = (char *)malloc((strlen(_infix) + 1) * sizeof(char));
    strcpy(infix, _infix);
    infixRegular(infix);
    struct Stack *numStack = Stack_createStack(0U);
    struct Stack *opStack = Stack_createStack(0U);

    size_t len = strlen(infix);
    long long currentNum = 0;
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
                currentNum = 0;
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
        currentNum = 0;
    }

    while (!Stack_isEmpty(opStack)) {
        struct Stack_Data *ret = Stack_pop(opStack);
        calcNumStack(numStack, ret->data.op);
        Stack_destroyData(ret);
    }

    struct Stack_Data *result = Stack_pop(numStack);
    long long r = result->data.number;
    free(infix);
    Stack_destroyData(result);
    Stack_destroyStack(numStack);
    Stack_destroyStack(opStack);

    return r;
}

void calcNumStack(struct Stack *numStack, char op) {
    long long a = 0;
    long long b = 0;

    struct Stack_Data *numRet = Stack_pop(numStack);
    b = numRet->data.number;
    Stack_destroyData(numRet);
    numRet = Stack_pop(numStack);
    a = numRet->data.number;
    Stack_destroyData(numRet);

    long long r = applyOp(op, a, b);

    struct Stack_Data result;
    result.data.number = r;
    Stack_push(numStack, &result);
}

void Stack_destroyData(struct Stack_Data *target) { free(target); }
