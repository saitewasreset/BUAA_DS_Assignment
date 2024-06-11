#include "ArrayList.h"
#include "Stack.h"
#include "trace.h"
#include <stdio.h>
#include <string.h>

#define BUFFERLEN 50

int main(void) {
    traceInit();
    struct ArrayList *functions = ArrayList_createList(100);

    struct Stack *stack = Stack_createStack(0);

    int op = 0;
    char buffer[BUFFERLEN] = {0};
    struct Stack_Data *ret = NULL;
    do {
        scanf("%d", &op);
        switch (op) {
        case 8:
            scanf("%s", buffer);
            char *arrayFunctionName =
                (char *)_malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(arrayFunctionName, buffer);

            struct ArrayList_Data ad = {0, arrayFunctionName, NULL};

            if (ArrayList_search(functions, &ad) == NULL) {
                struct ArrayList *called = ArrayList_createList(100);
                ad.called = called;
                ArrayList_insertItem(functions, ArrayList_getLength(functions),
                                     &ad);
            } else {
                _free(arrayFunctionName);
            }

            if (!Stack_isEmpty(stack)) {
                struct Stack_Data *caller = Stack_getTop(stack);
                struct ArrayList_Data search = {0, caller->functionName, NULL};
                struct ArrayList_Data *adCaller =
                    ArrayList_search(functions, &search);
                char *calledFunctionName =
                    (char *)_malloc((strlen(buffer) + 1) * sizeof(char));
                strcpy(calledFunctionName, buffer);
                struct ArrayList_Data calledInfo = {1, calledFunctionName,
                                                    NULL};
                if (ArrayList_search(adCaller->called, &calledInfo) == NULL) {
                    ArrayList_insertItem(adCaller->called,
                                         adCaller->called->len, &calledInfo);
                } else {
                    _free(calledFunctionName);
                }
            }

            char *stackFunctionName =
                (char *)malloc((strlen(buffer) + 1) * sizeof(char));
            strcpy(stackFunctionName, buffer);
            struct Stack_Data sd = {stackFunctionName};
            Stack_push(stack, &sd);
            break;
        case 0:
            ret = Stack_pop(stack);
            Stack_destroyData(ret);
            break;
        }

    } while (!Stack_isEmpty(stack));
    for (int i = 0; i < ArrayList_getLength(functions); i++) {
        struct ArrayList *calledList = functions->lst[i]->called;

        if (ArrayList_getLength(calledList) > 0) {
            printf("%s:", functions->lst[i]->functionName);
            for (int j = 0; j < ArrayList_getLength(calledList) - 1; j++) {
                printf("%s,", calledList->lst[j]->functionName);
            }
            printf("%s\n", calledList->lst[ArrayList_getLength(calledList) - 1]
                               ->functionName);
        }
    }
    for (int i = 0; i < ArrayList_getLength(functions); i++) {
        ArrayList_destroyList(functions->lst[i]->called);
    }

    ArrayList_destroyList(functions);
    Stack_destroyStack(stack);
    // traceSummarize();
    traceFree();
    return 0;
}

void ArrayList_destroyData(struct ArrayList_Data *target) {
    _free(target->functionName);
    _free(target);
}
void ArrayList_printData(struct ArrayList_Data *target) {
    if (target->type == 0) {
        printf("%s:", target->functionName);
        ArrayList_printList(target->called);
    } else {
        printf("%s", target->functionName);
    }
}
int ArrayList_dataEquals(struct ArrayList_Data *a, struct ArrayList_Data *b) {
    if (strcmp(a->functionName, b->functionName) == 0) {
        return 1;
    } else {
        return 0;
    }
}
int ArrayList_dataCmp(const void *a, const void *b) {
    struct ArrayList_Data *dA = *((struct ArrayList_Data **)a);
    struct ArrayList_Data *dB = *((struct ArrayList_Data **)b);
    int r = strcmp(dA->functionName, dB->functionName);
    if (r < 0) {
        return -1;
    } else if (r > 0) {
        return 1;
    } else {
        return 0;
    }
}

void Stack_destroyData(struct Stack_Data *target) {
    free(target->functionName);
    free(target);
}