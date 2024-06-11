#ifndef __H_RING
#define __H_RING
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct Node {
    char ch;
    struct Node *prev;
    struct Node *next;
};

struct Ring {
    struct Node *head;
    int len;
};

struct Ring *createRing(char * chLst, int len);
void destroyRing(struct Ring *target);
void printRing(struct Ring *target);
void deleteNode(struct Ring *ring, struct Node *node);
struct Node *generateKey(struct Ring *ring, struct Node *begin, char *map, char first);
void generateKeyMap(struct Ring *ring, char *map);
#endif