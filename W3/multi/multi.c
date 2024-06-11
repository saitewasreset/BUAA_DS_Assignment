#include "LinkedList.h"
#include <stdio.h>

#define MAXLEN 1005

struct LinkedList *getPoly();
int getData(struct Data *target);
void polyCombine(struct LinkedList *lst, struct Data data);
void polyMultiply(struct LinkedList *target, struct LinkedList *a,
                  struct LinkedList *b);

int main(void) {
    struct Data resultInit = {0, 0};
    struct LinkedList *polyA = getPoly();
    struct LinkedList *polyB = getPoly();
    struct LinkedList *result = createList(resultInit);
    polyMultiply(result, polyA, polyB);
    printLst(result);
    destroyList(polyA);
    destroyList(polyB);
    destroyList(result);
    return 0;
}

struct LinkedList *getPoly() {
    struct Data temp = {0, 0};
    int ret = getData(&temp);
    struct LinkedList *lst = createList(temp);
    if (ret == 0) {
        while (getData(&temp) != 1) {
            insertFirst(lst, temp);
        }
        insertFirst(lst, temp);
    }
    return lst;
}

int getData(struct Data *target) {
    int ch;
    int c = 0;
    int p = 0;
    while (1) {
        ch = getchar();
        if ((ch == EOF) || (ch == '\n')) {
            return -1;
        }
        if (ch == ' ') {
            break;
        }
        c = 10 * c + (ch - '0');
    }

    while (1) {
        ch = getchar();
        if ((ch == EOF) || (ch == '\n')) {
            target->c = c;
            target->p = p;
            return 1;
        }
        if (ch == ' ') {
            break;
        }
        p = 10 * p + (ch - '0');
    }
    target->c = c;
    target->p = p;
    return 0;
}

void polyCombine(struct LinkedList *lst, struct Data data) {
    struct Node *current = lst->head;
    while (current != NULL) {
        if (current->d.p == data.p) {
            current->d.c += data.c;
            return;
        }
        current = current->next;
    }
    insertOrdered(lst, data);
}

void polyMultiply(struct LinkedList *target, struct LinkedList *a,
                  struct LinkedList *b) {
    struct Node *currentA = a->head;
    while (currentA != NULL) {
        struct Node *currentB = b->head;
        while (currentB != NULL) {
            struct Data temp = {0, 0};
            temp.c = currentA->d.c * currentB->d.c;
            temp.p = currentA->d.p + currentB->d.p;
            polyCombine(target, temp);
            currentB = currentB->next;
        }
        currentA = currentA->next;
    }
}

void printData(struct Data *data) {
    if (data->c != 0) {
        printf("%d %d ", data->c, data->p);
    }
}

int dataEquals(struct Data *a, struct Data *b) {
    if ((a->c == b->c) && (a->p == b->p)) {
        return 1;
    }
    return 0;
}

int dataCmp(struct Data *a, struct Data *b) {
    if (a->p < b->p) {
        return 1;
    } else if (a->p > b->p) {
        return -1;
    } else {
        return 0;
    }
}