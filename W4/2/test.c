#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxsize 205
char stack[maxsize];
int stack1[maxsize];
int top = -1;
int empty() {
    if (top == -1) {
        return 1;
    }
    return 0;
}
void pushstack(char s, int i) {
    stack[++top] = s;
    stack1[top] = i;
}
void popstack() { top--; }
int main() {
    FILE *in;
    char ch, str[205];
    int flag = 0, l = 1, i = 0, line[205];
    in = fopen("example.c", "r");
    while ((ch = fgetc(in)) != EOF) { // 将符合条件的括号读取到str字符串中
        if (ch == '/') {
            ch = fgetc(in);
            if (ch == '/') { // 注释一
                flag = 1;
            } else if (ch == '*') { // 注释二
                flag = 2;
            } else {
                ungetc(ch, in);
            }
            continue;
        } else if (ch == '\n') {
            if (flag == 1) { // 注释一结束标志
                flag = 0;
            }
            l++;                                 // 记录行数
        } else if ((ch == '*') && (flag == 2)) { // 注释二结束标志
            ch = fgetc(in);
            if (ch == '/') {
                flag = 0;
                continue;
            } else {
                ungetc(ch, in);
            }
        } else if (ch == '"') { // 处理printf函数
            if (flag == 0) {
                flag = 3;
            } else if (flag == 3) {
                flag = 0;
            } else {
                continue;
            }
        }
        if ((ch == '(' || ch == ')' || ch == '{' || ch == '}') &&
            (flag == 0)) { // 读入字符串
            str[i] = ch;
            line[i] = l;
            i++;
        } else {
            continue;
        }
    }
    str[i] = '\0';
    i = 0;
    flag = 0;
    while (str[i] != '\0') { // 遍历str，进行入栈出栈以及纠错环节
        if (str[i] == '(' || str[i] == '{') {
            if ((str[i] == '{') && (stack[top] == '(')) {
                flag = 1;
                printf("without maching '%c' at line %d", stack[top],
                       stack1[top]);
            } else {
                pushstack(str[i], line[i]);
            }
        } else {
            if (str[i] == ')') {
                if ((stack[top] == '(') && (empty() == 0)) {
                    popstack();
                } else {
                    flag = 1;
                    printf("without maching '%c' at line %d", str[i], line[i]);
                }
            } else {
                if ((stack[top] == '{') && (empty() == 0)) {
                    popstack();
                } else {
                    flag = 1;
                    printf("without maching '%c' at line %d", str[i], line[i]);
                }
            }
        }
        i++;
        if (flag) {
            break;
        }
    }
    if (flag == 0) {
        if (empty()) {
            printf("%s", str);
        } else {
            printf("without maching '%c' at line %d", stack[top], stack1[top]);
        }
    }
    return 0;
}