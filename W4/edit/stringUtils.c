#include "stringUtils.h"
#include <stdio.h>
#include <string.h>

// 以下计数从0开始
// 从i（含）开始截断
void String_truncate(char *str, size_t i) { str[i] = '\0'; }
// 将a插入到target的第i位之后
void String_insert(char *target, char *a, long i) {
    char temp[MAXLEN] = {0};
    strcpy(temp, target + i + 1);
    target[i + 1] = '\0';
    strcat(target, a);
    strcat(target, temp);
}

// 将 target 第 i 位到第 j 位（包括第 i, j 位） 之间的内容删除
void String_delete(char *target, size_t i, size_t j) {
    char temp[MAXLEN] = {0};
    strcpy(temp, target + j + 1);
    target[i] = '\0';
    strcat(target, temp);
}

// 将 target 第 i 位到第 j 位（包括第 i, j 位） 之间的内容替换为 str

void String_replace(char *target, char *str, size_t i, size_t j) {
    char temp[MAXLEN] = {0};
    str[i] = '\0';
    strcpy(temp, str + j + 1);
    strcat(target, str);
    strcat(target, temp);
}

// 若 target 存在于 str 中，输出 target 在 str
// 中第一次出现和最后一次出现的位置，用一个空格分开。 若不存在，输出一行 NULL
void String_fullFind(char *str, char *target) {
    char *find = strstr(str, target);
    char *begin = find + 1;
    char *first = NULL;
    char *last = NULL;
    if (find == NULL) {
        printf("NULL\n");
    } else {
        first = find;
        last = find;
        while ((find = strstr(begin, target)) != NULL) {
            last = find;
            begin++;
        }
        printf("%zu %zu\n", (first - str), (last - str));
    }
}
