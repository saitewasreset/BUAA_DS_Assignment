#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

#define THRESHOLD 7
#define MIN_WINDOW_NUM 3
#define DEFAULT_WINDOW_NUM 3
#define MAX_WINDOW_NUM 5

size_t sum(size_t *arr, size_t len);
bool process(struct Queue *q, size_t *windows, unsigned currentWindowNum);
void updateWaitTime(struct Queue_Data *target);

int main(void) {
    size_t clock = 0;
    scanf("%zu", &clock);

    size_t *customerCount = (size_t *)malloc(clock * sizeof(size_t));

    for (size_t i = 0; i < clock; i++) {
        scanf("%zu", &customerCount[i]);
    }
    size_t maxQueueLen = sum(customerCount, clock);

    struct Queue *queue = Queue_createQueue(maxQueueLen);
    size_t windows[MAX_WINDOW_NUM] = {0};
    size_t currentCustomerId = 1;
    unsigned currentWindowNum = DEFAULT_WINDOW_NUM;
    for (size_t i = 0; i < clock; i++) {
        for (size_t j = 0; j < customerCount[i]; j++) {
            struct Queue_Data d;
            scanf("%zu", &d.remainProcessTime);
            d.id = currentCustomerId;
            d.waitTime = 0;
            Queue_enQueue(queue, &d);
            currentCustomerId++;
        }

        while (Queue_getLength(queue) >= THRESHOLD * currentWindowNum) {
            if (currentWindowNum < MAX_WINDOW_NUM) {
                windows[currentWindowNum] = 0;
                currentWindowNum++;
            } else {
                break;
            }
        }

        if (process(queue, windows, currentWindowNum)) {
            while (Queue_getLength(queue) < THRESHOLD * currentWindowNum) {

                if (currentWindowNum > MIN_WINDOW_NUM) {
                    /*
                    if (currentWindowNum == MAX_WINDOW_NUM) {
                        if (windows[currentWindowNum - 1] <
                            windows[currentWindowNum - 2]) {
                            windows[currentWindowNum - 2] =
                                windows[currentWindowNum - 1];
                        }
                    }*/
                    currentWindowNum--;

                } else {
                    break;
                }
            }
        }

        Queue_forEach(queue, updateWaitTime);
        for (size_t j = 0; j < currentWindowNum; j++) {
            if (windows[j] > 0) {
                windows[j]--;
            }
        }
    }

    while (!Queue_isEmpty(queue)) {
        if (process(queue, windows, currentWindowNum)) {
            while (Queue_getLength(queue) < THRESHOLD * currentWindowNum) {

                if (currentWindowNum > MIN_WINDOW_NUM) {
                    /*
                    if (currentWindowNum == MAX_WINDOW_NUM) {
                        if (windows[currentWindowNum - 1] <
                            windows[currentWindowNum - 2]) {
                            windows[currentWindowNum - 2] =
                                windows[currentWindowNum - 1];
                        }
                    }*/
                    currentWindowNum--;
                } else {
                    break;
                }
            }
        }

        Queue_forEach(queue, updateWaitTime);
        for (size_t i = 0; i < currentWindowNum; i++) {
            if (windows[i] > 0) {
                windows[i]--;
            }
        }
    }

    Queue_destroyQueue(queue);
    free(customerCount);
    return 0;
}

size_t sum(size_t *arr, size_t len) {
    size_t result = 0;
    for (size_t i = 0; i < len; i++) {
        result += arr[i];
    }
    return result;
}

void Queue_destroyData(struct Queue_Data *target) { free(target); }
bool process(struct Queue *q, size_t *windows, unsigned currentWindowNum) {
    bool processed = false;
    for (unsigned i = 0; i < currentWindowNum; i++) {
        if ((!Queue_isEmpty(q)) && (windows[i] == 0)) {
            processed = true;
            struct Queue_Data *ret = Queue_deQueue(q);
            windows[i] = ret->remainProcessTime;
            printf("%zu : %zu\n", ret->id, ret->waitTime);
            Queue_destroyData(ret);
        }
    }
    return processed;
}

void updateWaitTime(struct Queue_Data *target) { target->waitTime++; }