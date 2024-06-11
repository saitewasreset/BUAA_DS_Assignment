#include "alloc.h"

struct Alloc *createAlloc(struct Data **dataLst, int len) {
    struct Alloc *result = (struct Alloc *)malloc(sizeof(struct Alloc));
    result->len = len;

    struct Node *current = (struct Node *)malloc(sizeof(struct Node));
    current->d = *dataLst[0];
    current->next = NULL;
    result->current = current;
    for (int i = 1; i < len; i++) {
        current->next = (struct Node *)malloc(sizeof(struct Node));
        current->next->d = *dataLst[i];
        current->next->next = NULL;
        current = current->next;
    }
    current->next = result->current;
    return result;
}

void destroyAlloc(struct Alloc *target) {
    struct Node *current = target->current;
    for (int i = 0; i < target->len; i++) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
    free(target);
}

int alloc(struct Alloc *target, int size) {
    struct Node *_current = target->current;
    struct Node *current = target->current;

    int minSize = INT_MAX;
    struct Node *minNode = NULL;
    struct Node *minNodePrev = NULL;

    int currentSize = current->d.size;

    while (current->next != _current) {
        if ((current->next->d.size >= size) &&
            (current->next->d.size < minSize)) {
            minSize = current->next->d.size;
            minNode = current->next;
            minNodePrev = current;
        }
        current = current->next;
    }

    if ((current->next->d.size >= size) && (current->next->d.size < minSize)) {
        minSize = current->next->d.size;
        minNode = current->next;
        minNodePrev = current;
    }
    current = current->next;

    if (minSize == currentSize) {
        struct Node *currentPrev = current;
        if (currentSize > size) {
            current->d.size -= size;

        } else {
            while (currentPrev->next != current) {
                currentPrev = currentPrev->next;
            }
            currentPrev->next = current->next;
            target->current = current->next;
            free(current);
            target->len--;
        }
        return 1;
    }

    if (minNode == NULL) {
        return 0;
    }

    if (minNode->d.size == size) {
        minNodePrev->next = minNode->next;
        free(minNode);
        target->len--;
        target->current = minNodePrev->next;
    } else {
        minNode->d.size -= size;
        target->current = minNode;
    }

    return 1;
}

void printAlloc(struct Alloc *target) {
    struct Node *current = target->current;
    for (int i = 0; i < target->len; i++) {
        printf("%d %d\n", current->d.address, current->d.size);
        current = current->next;
    }
}