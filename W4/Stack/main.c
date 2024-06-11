#include "Stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int op = 0;
    int data = 0;

    struct Stack *stack = Stack_createStack((unsigned)100);

    while (1) {
        scanf("%d", &op);
        if (op == 1) {
            scanf("%d", &data);
            struct Stack_Data d = {data};
            if (!Stack_push(stack, &d)) {
                printf("error ");
            }
        } else if (op == 0) {
            struct Stack_Data *d = Stack_pop(stack);
            if (d == NULL) {
                printf("error ");
            } else {
                printf("%d ", d->n);
                Stack_destroyData(d);
            }
        } else {
            break;
        }
    }
    Stack_destroyStack(stack);
    return 0;
}

void Stack_destroyData(struct Stack_Data *target) { free(target); }