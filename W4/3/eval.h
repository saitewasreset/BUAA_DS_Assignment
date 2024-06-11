#ifndef __EVAL_H
#define __EVAL_H

#include <stdbool.h>

#define TOP_OP_OPEN '('
#define TOP_OP_CLOSE ')'
// 无空格
void infixRegular(char *infix);
// 数字以单个空格分隔，数字与运算符间无空格
void RPNRegular(char *s);

// Auto regular input
double RPNCalc(char *s);
void infixToRPN(char *infix, char *target);
double eval(char *infix);

bool isOp(char c);
double applyOp(char op, double a, double b);
int opCmp(char a, char b);

#endif
