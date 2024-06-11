#ifndef __CIRCULARLINKEDLIST_H__
#define __CIRCULARLINKEDLIST_H__

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

struct CircularLinkedList {
    struct Node *head;
    int len;
};

// index 可大于链表长度，将循环

struct CircularLinkedList *createList(struct Data initData);
int getLength(struct CircularLinkedList *lst);
void destroyList(struct CircularLinkedList *lst);
void printLst(struct CircularLinkedList *lst);
void printLstWithNum(struct CircularLinkedList *lst, int num);
void insert(struct CircularLinkedList *lst, struct Node *target, struct Data data);
void insertFirst(struct CircularLinkedList *lst, struct Data data);
int insertIndex(struct CircularLinkedList *lst, int index, struct Data data);
struct Node *searchNode(struct CircularLinkedList *lst, struct Data data);
struct Node *searchNodeBeginAt(struct CircularLinkedList *lst,
                               struct Node *begin, struct Data data);
void deleteNode(struct CircularLinkedList *lst, struct Node *node);
int deleteIndex(struct CircularLinkedList *lst, int index);
struct Node *getIndex(struct CircularLinkedList *lst, int index);
struct Node *seekNode(struct Node *source, int step);
#endif