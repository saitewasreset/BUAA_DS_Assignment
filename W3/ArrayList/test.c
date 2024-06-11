#include "ArrayList.h"
#include <string.h>

int main(void) {
    struct Data d1 = {"1", 5};
    struct Data d2 = {"2", 4};
    struct Data d3 = {"3", 3};
    struct Data d4 = {"4", 2};
    struct Data d5 = {"5", 1};
    struct ArrayList *lst = createList(2);
    printf("len = %d\n", lst->len);
    printList(lst);
    listInsertItem(lst, 0, d5);
    listInsertItem(lst, 0, d4);
    listInsertItem(lst, 0, d3);
    listInsertItem(lst, 0, d2);
    listInsertItem(lst, 0, d1);
    /*
    listInsertItemOrdered(lst, d5);
    listInsertItemOrdered(lst, d4);
    listInsertItemOrdered(lst, d3);
    listInsertItemOrdered(lst, d2);
    listInsertItemOrdered(lst, d1);
    */
    printf("len = %d\n", lst->len);
    printList(lst);
    printf("d1 pos: %d\n", listGetPosOrdered(lst, &d1));
    printf("d2 pos: %d\n", listGetPosOrdered(lst, &d2));
    printf("d3 pos: %d\n", listGetPosOrdered(lst, &d3));
    printf("d4 pos: %d\n", listGetPosOrdered(lst, &d4));
    printf("d5 pos: %d\n", listGetPosOrdered(lst, &d5));
    listSearchOrdered(lst, &d3)->count++;
    printf("len = %d\n", lst->len);
    printList(lst);
    listDeleteItem(lst, 4);
    listDeleteItem(lst, 0);
    printf("len = %d\n", lst->len);
    printList(lst);
    listSearch(lst, d3)->count++;
    printf("len = %d\n", lst->len);
    printList(lst);
    listSort(lst, dataCmp);
    printf("len = %d\n", lst->len);
    printList(lst);
    destroyList(lst);
    return 0;
}

void destroyData(struct Data *target) { _free(target); }
void printData(struct Data *target) {
    printf("{word = \"%s\", count = %d}", target->word, target->count);
}
int dataEquals(struct Data *a, struct Data *b) {
    if (strcmp(a->word, b->word) == 0) {
        return 1;
    } else {
        return 0;
    }
}
int dataCmp(const void *a, const void *b) {
    struct Data *dA = *((struct Data **)a);
    struct Data *dB = *((struct Data **)b);
    return strcmp(dA->word, dB->word);
}
