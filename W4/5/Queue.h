#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdbool.h>
#include <stddef.h>

struct Queue_Data {
    size_t id;
    size_t remainProcessTime;
    size_t waitTime;
};

void Queue_destroyData(struct Queue_Data *target);

struct Queue {
    struct Queue_Data **lst;
    size_t front;
    size_t rear;
    size_t count;
    size_t capacity;
};

struct Queue *Queue_createQueue(size_t capacity);
void Queue_destroyQueue(struct Queue *target);
bool Queue_enQueue(struct Queue *q, struct Queue_Data *element);
struct Queue_Data *Queue_deQueue(struct Queue *q);
struct Queue_Data *Queue_getHead(struct Queue *q);
bool Queue_isFull(struct Queue *q);
bool Queue_isEmpty(struct Queue *q);
void Queue_forFirstN(struct Queue *q, size_t n, void (*f)(struct Queue_Data *));
void Queue_forEach(struct Queue *q, void (*f)(struct Queue_Data *));
size_t Queue_getLength(struct Queue *q);

#endif
