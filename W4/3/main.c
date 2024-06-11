#include "eval.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAXLEN 205

char *s_gets(char *dst, int n);

int main(void) {
    char buffer[MAXLEN] = {0};
    s_gets(buffer, MAXLEN);
    double result = eval(buffer);
    printf("%.2f\n", result);
    return 0;
}

char *s_gets(char *dst, int n) {
    char *ret = fgets(dst, n, stdin);
    int i = 0;
    if (ret != NULL) {
        while (dst[i] != '\n' && dst[i] != '\0') {
            i++;
        }
        if (dst[i] == '\n') {
            dst[i] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                continue;
            }
        }
    }

    return ret;
}

void infixRegular(char *infix) {
    char buffer[MAXLEN] = {0};
    size_t len = strlen(infix);
    size_t index = 0;
    // skip '='
    for (size_t i = 0; (i < len - 1); i++) {
        if (!isspace(infix[i])) {
            buffer[index] = infix[i];
            index++;
        }
    }
    buffer[index] = '\0';
    strcpy(infix, buffer);
}
void RPNRegular(char *s) {}
bool isOp(char c) {
    if ((c == '+') || (c == '-') || (c == '*') || (c == '/')) {
        return true;
    }
    return false;
}
double applyOp(char op, double a, double b) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
    return 0.0;
}
int opCmp(char a, char b) {
    if ((a == '+') || (a == '-')) {
        if ((b == '+') || (b == '-')) {
            return 0;
        } else {
            return -1;
        }
    }
    if ((a == '*') || (a == '/')) {
        if ((b == '+') || (b == '-')) {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}
