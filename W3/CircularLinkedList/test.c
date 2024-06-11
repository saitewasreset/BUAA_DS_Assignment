#include "CircularLinkedList.h"
#include "trace/trace.h"
#include <stdio.h>
/*

*/
int main(void) {
    traceInit();
    struct Data d1 = {5, 1};
    struct Data d2 = {4, 2};
    struct Data d3 = {3, 3};
    struct Data d4 = {2, 4};
    struct Data d5 = {1, 5};
    struct CircularLinkedList *lst = createList(d1);
    //    insertIndex(lst, 0, d1);
    insertIndex(lst, 0, d2);
    insertIndex(lst, 0, d3);
    insertIndex(lst, 0, d4);
    insertIndex(lst, 0, d5);
    insertIndex(lst, 5, d5);
    printf("Len = %d\n", getLength(lst));
    printLst(lst);
    printf("Num: 10\n");
    printLstWithNum(lst, 10);

    searchNode(lst, d3)->d.c++;
    searchNode(lst, d5)->d.c++;
    printf("Len = %d\n", getLength(lst));
    printLst(lst);

    struct Node *nodeD3 = searchNode(lst, d3);
    searchNodeBeginAt(lst, nodeD3, d5)->d.c++;
    printf("Len = %d\n", getLength(lst));
    printLst(lst);

    printf("Index: 10\n");
    printData(&getIndex(lst, 10)->d);
    putchar('\n');

    deleteIndex(lst, 0);
    deleteIndex(lst, 5);
    printf("Len = %d\n", getLength(lst));
    printLst(lst);

    printf("d3 + 10\n");
    printData(&seekNode(nodeD3, 10)->d);
    putchar('\n');

    destroyList(lst);
    traceSummarize();
    traceFree();
    return 0;
}

void printData(struct Data *data) {
    printf("{c = %d, p = %d}", data->c, data->p);
}
int dataEquals(struct Data *a, struct Data *b) {
    if (a->p == b->p) {
        return 1;
    } else {
        return 0;
    }
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