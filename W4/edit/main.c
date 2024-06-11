#include "Stack.h"
#include "stringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN 550

char str[STRLEN] = {0};

char *s_gets(char *dst, int n);
void doEdit(char *target, struct Stack_Data *opInfo);
void doReverse(char *target, struct Stack_Data *opInfo);
size_t min(size_t a, size_t b);

int main(void) {
    s_gets(str, STRLEN);
    size_t n = 0;
    scanf("%zu", &n);

    struct Stack *edit = Stack_createStack(0U);

    for (size_t i = 0; i < n; i++) {
        char buffer[STRLEN] = {0};
        int op = 0;
        size_t pos = 0;

        scanf("%d%zu%s", &op, &pos, buffer);

        size_t bufferLen = strlen(buffer);

        char *content = (char *)malloc((bufferLen + 1) * sizeof(char));
        strcpy(content, buffer);
        struct Stack_Data d = {op, pos, content};

        Stack_push(edit, &d);
    }

    /*
    struct Stack_Data *current = NULL;
    while ((current = Stack_pop(edit)) != NULL) {
        doReverse(str, current);
        Stack_destroyData(current);
    }
    puts(str);
    */

    int op = -1;
    while (scanf("%d", &op) != EOF) {
        struct Stack_Data current;
        size_t pos = 0;
        char buffer[MAXLEN] = {0};

        switch (op) {
        case -1:
            break;
        case 1:
            current.op = 1;
            scanf("%zu %s", &pos, buffer);
            current.pos = pos;
            current.content =
                (char *)malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(current.content, buffer);
            doEdit(str, &current);
            Stack_push(edit, &current);
            break;
        case 2:
            current.op = 2;
            size_t deleteNum = 0;
            scanf("%zu%zu", &pos, &deleteNum);

            size_t strLen = strlen(str);
            deleteNum = min(deleteNum, strLen - pos);

            current.pos = pos;
            current.content = (char *)malloc((deleteNum + 1) * sizeof(char));
            for (size_t i = 0; i < deleteNum; i++) {
                current.content[i] = str[pos + i];
            }
            current.content[deleteNum] = '\0';
            doEdit(str, &current);
            Stack_push(edit, &current);
            break;
        case 3:
            if (!Stack_isEmpty(edit)) {
                struct Stack_Data *ret = Stack_pop(edit);
                doReverse(str, ret);
                Stack_destroyData(ret);
            }
            break;
        }
    }
    puts(str);
    Stack_destroyStack(edit);

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

void Stack_destroyData(struct Stack_Data *target) {
    free(target->content);
    free(target);
}

void doReverse(char *target, struct Stack_Data *opInfo) {
    switch (opInfo->op) {
    case 1:
        String_delete(target, opInfo->pos,
                      opInfo->pos + strlen(opInfo->content) - 1);
        break;
    case 2:
        String_insert(target, opInfo->content, (long)opInfo->pos - 1);
        break;
    }
}

void doEdit(char *target, struct Stack_Data *opInfo) {
    switch (opInfo->op) {
    case 1:
        String_insert(target, opInfo->content, (long)opInfo->pos - 1);
        break;
    case 2:
        String_delete(target, opInfo->pos,
                      opInfo->pos + strlen(opInfo->content) - 1);
        break;
    }
}

inline size_t min(size_t a, size_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}