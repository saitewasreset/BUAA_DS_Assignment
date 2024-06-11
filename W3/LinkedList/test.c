#include "LinkedList.h"
#include <stdio.h>

int main(void) {
    traceInit();
    struct Data d1 = {0, 6};
    struct Data d2 = {0, 5};
    struct Data d3 = {0, 4};
    struct Data d4 = {0, 3};
    struct Data d5 = {0, 3};
    struct Data d6 = {0, 3};
    struct Data d7 = {0, 2};
    struct Data d8 = {0, 1};
    struct LinkedList *lst = createList(d1);
    insertOrdered(lst, d2);
    insertOrdered(lst, d3);
    insertOrdered(lst, d4);
    insertOrdered(lst, d5);
    insertOrdered(lst, d6);
    insertOrdered(lst, d7);
    insertOrdered(lst, d8);
    printf("len = %d\n", getLength(lst));
    printLst(lst);
    destroyList(lst);
    traceSummarize();
    traceFree();
    return 0;
}

void printData(struct Data *data) {
    printf("{c = %d p = %d}\n", data->c, data->p);
}

int dataEquals(struct Data *a, struct Data *b) {
    if ((a->c == b->c) && (a->p == b->p)) {
        return 1;
    }
    return 0;
}

int dataCmp(struct Data *a, struct Data *b) {
    if (a->p < b->p) {
        return -1;
    } else if (a->p > b->p) {
        return 1;
    } else {
        return 0;
    }
}