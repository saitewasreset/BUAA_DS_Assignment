#include "Queue.h"
#include <stdlib.h>
#include <string.h>

size_t min(size_t a, size_t b);

struct Queue *Queue_createQueue(size_t capacity) {
    struct Queue *result = (struct Queue *)malloc(sizeof(struct Queue));
    result->lst =
        (struct Queue_Data **)malloc(capacity * sizeof(struct Queue_Data *));
    for (size_t i = 0; i < capacity; i++) {
        result->lst[i] = NULL;
    }
    result->capacity = capacity;
    result->front = 0;
    result->rear = capacity - 1;
    result->count = 0;
    return result;
}
void Queue_destroyQueue(struct Queue *target) {
    for (size_t i = 0; i < target->capacity; i++) {
        if (target->lst[i] != NULL) {
            Queue_destroyData(target->lst[i]);
        }
    }
    free(target->lst);
    free(target);
}
bool Queue_enQueue(struct Queue *q, struct Queue_Data *element) {
    if (Queue_isFull(q)) {
        return false;
    }
    struct Queue_Data *_element =
        (struct Queue_Data *)malloc(sizeof(struct Queue_Data));
    memcpy(_element, element, sizeof(struct Queue_Data));

    q->rear = (q->rear + 1) % q->capacity;
    q->lst[q->rear] = _element;
    q->count++;

    return true;
}
struct Queue_Data *Queue_deQueue(struct Queue *q) {
    if (Queue_isEmpty(q)) {
        return NULL;
    }
    struct Queue_Data *ret = q->lst[q->front];
    q->lst[q->front] = NULL;
    q->front = (q->front + 1) % q->capacity;
    q->count--;
    return ret;
}

struct Queue_Data *Queue_getHead(struct Queue *q) {
    if (Queue_isEmpty(q)) {
        return NULL;
    } else {
        return q->lst[q->front];
    }
}

bool Queue_isFull(struct Queue *q) { return (bool)(q->count == q->capacity); }
bool Queue_isEmpty(struct Queue *q) { return (bool)(q->count == 0); }

void Queue_forFirstN(struct Queue *q, size_t n,
                     void (*f)(struct Queue_Data *)) {
    for (size_t i = 0; i < min(n, q->count); i++) {
        f(q->lst[(q->front + i) % q->capacity]);
    }
}

void Queue_forEach(struct Queue *q, void (*f)(struct Queue_Data *)) {
    Queue_forFirstN(q, q->count, f);
}

size_t Queue_getLength(struct Queue *q) { return q->count; }

inline size_t min(size_t a, size_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}