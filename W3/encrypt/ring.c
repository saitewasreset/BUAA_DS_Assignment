#include "ring.h"

struct Ring *createRing(char *chLst, int len) {
    struct Ring *result = (struct Ring *)malloc(sizeof(struct Ring));
    result->len = len;

    struct Node *current = (struct Node *)malloc(sizeof(struct Node));
    current->ch = chLst[0];
    current->prev = NULL;
    current->next = NULL;
    result->head = current;
    for (int i = 1; i < len; i++) {
        current->next = (struct Node *)malloc(sizeof(struct Node));
        current->next->ch = chLst[i];
        current->next->next = NULL;
        current->next->prev = current;
        current = current->next;
    }
    current->next = result->head;
    result->head->prev = current;
    return result;
}

void destroyRing(struct Ring *target) {
    struct Node *current = target->head;
    for (int i = 0; i < target->len; i++) {
        struct Node *next = current->next;
        free(current);
        current = next;
    }
    free(target);
}

void printRing(struct Ring *target) {
    struct Node *current = target->head;
    for (int i = 0; i < target->len; i++) {
        printf("%c", current->ch);
        current = current->next;
    }
}

void deleteNode(struct Ring *ring, struct Node *node) {
    struct Node *prev = node->prev;
    struct Node *next = node->next;
    prev->next = next;
    next->prev = prev;
    if (node == ring->head) {
        ring->head = next;
    }

    free(node);

    ring->len--;
}

void generateKeyMap(struct Ring *ring, char *map) {
    char first = ring->head->ch;
    struct Node *current = ring->head;
    while ((current = generateKey(ring, current, map, first)) != NULL) {
        continue;
    }
}

struct Node *generateKey(struct Ring *ring, struct Node *begin, char *map,
                         char first) {
    if (ring->len >= 2) {
        struct Node *current = begin;
        char source = current->ch;

        struct Node *mapped = current->next;
        deleteNode(ring, current);
        for (int i = 1; i < source; i++) {
            mapped = mapped->next;
        }

        map[(unsigned)(source)] = mapped->ch;
        return mapped;
    } else {
        map[(unsigned)(begin->ch)] = first;
        return NULL;
    }
}