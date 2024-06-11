#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Queue.h"
#include "Stack.h"

#define MAXLINELEN 205
#define MAXSTORELEN 300U

void read();

int main(void) {
    read();
    return 0;
}

void read() {
    char buffer[MAXLINELEN] = {0};
    FILE *in = fopen("example.c", "r");
    size_t line = 1;
    struct Stack *stack = Stack_createStack(MAXSTORELEN);
    struct Queue *queue = Queue_createQueue(MAXSTORELEN);

    bool inMultipleComment = false;
    bool inChar = false;
    bool inString = false;

    while (fgets(buffer, MAXLINELEN, in) != NULL) {

        size_t len = strlen(buffer);
        for (size_t i = 0; i < len; i++) {
            // Comment
            if (inMultipleComment) {
                if (i < (len - 1)) {
                    if ((buffer[i] == '*') && (buffer[i + 1]) == '/') {
                        inMultipleComment = false;
                        // skip next '/'
                        i++;
                    } else {
                        continue;
                    }
                } else {
                    continue;
                }
            }
            if (buffer[i] == '/') {
                if (i < (len - 1)) {
                    if (buffer[i + 1] == '/') {
                        break;
                    } else if (buffer[i + 1] == '*') {
                        inMultipleComment = true;
                        continue;
                    }
                }
            }
            // char/string, without \', \"
            if (!inChar && !inString) {
                if (buffer[i] == '\'') {
                    inChar = true;
                    continue;
                } else if (buffer[i] == '\"') {
                    inString = true;
                    continue;
                }
            }

            if (inChar) {
                if (buffer[i] == '\'') {
                    inChar = false;
                    continue;
                }
            }

            if (inString) {
                if (buffer[i] == '\"') {
                    inString = false;
                    continue;
                }
            }

            if (inChar || inString) {
                continue;
            }

            if ((buffer[i] == '{') || (buffer[i] == '}') ||
                (buffer[i] == '(') || (buffer[i] == ')')) {
                struct Stack_Data current = {buffer[i], line};
                Queue_enQueue(queue, (struct Queue_Data *)&current);
                if (Stack_isEmpty(stack)) {
                    Stack_push(stack, &current);
                } else {
                    struct Stack_Data *top = Stack_getTop(stack);
                    if (buffer[i] == '{') {
                        if (top->sign == '{') {
                            Stack_push(stack, &current);
                        } else if (top->sign == '(') {
                            printf("without maching '(' at line %zu",
                                   top->line);
                            goto finish;
                        } else {
                            // printf("i = { i - 1 = %c NOT POSSIBLE!",
                            // top->sign);
                            goto finish;
                        }
                    } else if (buffer[i] == '}') {
                        if (top->sign == '{') {
                            struct Stack_Data *ret = Stack_pop(stack);
                            Stack_destroyData(ret);
                        } else {
                            printf("without maching '}' at line %zu", line);
                            goto finish;
                        }
                    } else if (buffer[i] == '(') {
                        Stack_push(stack, &current);
                    } else if (buffer[i] == ')') {
                        if (top->sign == '(') {
                            struct Stack_Data *ret = Stack_pop(stack);
                            Stack_destroyData(ret);
                        } else {
                            printf("without maching ')' at line %zu", line);
                            goto finish;
                        }
                    }
                }
            }
        }
        line++;
    }

    if (!Stack_isEmpty(stack)) {
        struct Stack_Data *ret = Stack_pop(stack);
        printf("without maching '%c' at line %zu\n", ret->sign, ret->line);
        Stack_destroyData(ret);
    } else {
        struct Queue_Data *ret = NULL;
        while ((ret = Queue_deQueue(queue)) != NULL) {
            printf("%c", ret->sign);
            Queue_destroyData(ret);
        }
    }
finish:
    Stack_destroyStack(stack);
    Queue_destroyQueue(queue);
    fclose(in);
}

void Stack_destroyData(struct Stack_Data *target) { free(target); }
void Queue_destroyData(struct Queue_Data *target) { free(target); }