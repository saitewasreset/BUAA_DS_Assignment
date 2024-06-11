#include "AbstractBinaryTree.h"
#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(x) (void)(x)
#define MAXINPUTLEN 1005

char *s_gets(char *dst, int n);

int main(void) {
    char buffer[MAXINPUTLEN] = {0};
    s_gets(buffer, MAXINPUTLEN);

    size_t inputLen = strlen(buffer);
    buffer[inputLen - 1] = '\0';

    char RPN[MAXINPUTLEN] = {0};

    infixToRPN(buffer, RPN);

    struct BinaryTree *tree = BinaryTree_fromRPN(RPN);

    if (isOp((char)*(tree->root->data))) {
        printf("%c", (int)*(tree->root->data));
    } else {
        printf("%lld", *(tree->root->data));
    }

    putchar(' ');
    if (isOp((char)*(tree->root->left->data))) {
        printf("%c", (int)*(tree->root->left->data));
    } else {
        printf("%lld", *(tree->root->left->data));
    }
    putchar(' ');
    if (isOp((char)*(tree->root->right->data))) {
        printf("%c", (int)*(tree->root->right->data));
    } else {
        printf("%lld", *(tree->root->right->data));
    }
    putchar('\n');

    long long result = RPNCalc(RPN);
    printf("%lld", result);
    BinaryTree_destroyTree(tree);

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

bool isOp(char c) {
    if ((c == '+') || (c == '-') || (c == '*') || (c == '/')) {
        return true;
    }
    return false;
}
long long applyOp(char op, long long a, long long b) {
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

BinaryTree_Data *BinaryTree_copyData(BinaryTree_Data *source) {
    BinaryTree_Data *result =
        (BinaryTree_Data *)malloc(sizeof(BinaryTree_Data));
    *result = *source;
    return result;
}
void BinaryTree_destroyData(BinaryTree_Data *target) { free(target); }

void BinaryTree_printData(BinaryTree_Data *target) { UNUSED(target); }

void infixRegular(char *infix) { UNUSED(infix); }
void RPNRegular(char *s) { UNUSED(s); }