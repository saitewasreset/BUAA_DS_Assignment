#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdlib.h>
#include "trace/trace.h"
// Impl by user
struct Data {
    int c;
    int p;
};
void printData(struct Data *data);
int dataEquals(struct Data *a, struct Data *b);
int dataCmp(struct Data *a, struct Data *b);
// --------------

struct Node {
    struct Data d;
    struct Node *next;
};

struct LinkedList {
    struct Node *head;
    int len;
};

struct LinkedList *createList(struct Data initData);
struct LinkedList *createListWithNum(int n);
int getLength(struct LinkedList *lst);
void destroyList(struct LinkedList *lst);
void printLst(struct LinkedList *lst);
void insert(struct LinkedList *lst, struct Node *target, struct Data data);
void insertFirst(struct LinkedList *lst, struct Data data);
void insertOrdered(struct LinkedList *lst, struct Data data);
int insertIndex(struct LinkedList *lst, int index, struct Data data);
struct Node *searchNode(struct LinkedList *lst, struct Data data);
struct Node *searchNodeBeginAt(struct Node *begin, struct Data data);
void deleteNode(struct LinkedList *lst, struct Node *node);
int deleteIndex(struct LinkedList *lst, int index);
struct Node *getIndex(struct LinkedList *lst, int index);
#endif