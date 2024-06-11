#ifndef __H_ALLOC
#define __H_ALLOC
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

struct Data {
    int address;
    int size;
};

struct Node {
    struct Data d;
    struct Node *next;
};

struct Alloc {
    struct Node *current;
    int len;
};

struct Alloc *createAlloc(struct Data **dataLst, int len);
void destroyAlloc(struct Alloc *target);
int alloc(struct Alloc *target, int size);
void printAlloc(struct Alloc *target);
#endif