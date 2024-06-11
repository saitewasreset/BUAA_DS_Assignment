#include "CircularLinkedList.h"
#include <stdio.h>

struct CircularLinkedList *createList(struct Data initData) {
    struct CircularLinkedList *result =
        (struct CircularLinkedList *)_malloc(sizeof(struct CircularLinkedList));
    result->len = 1;

    struct Node *head = (struct Node *)_malloc(sizeof(struct Node));
    head->next = head;

    result->head = head;
    head->d = initData;

    return result;
}

int getLength(struct CircularLinkedList *lst) { return lst->len; }

void destroyList(struct CircularLinkedList *lst) {
    struct Node *this = lst->head;
    for (int i = 0; i < lst->len; i++) {
        struct Node *temp = this->next;
        _free(this);
        this = temp;
    }
    _free(lst);
}

void printLst(struct CircularLinkedList *lst) {
    struct Node *this = lst->head;
    if (lst->len == 0) {
        printf("NULL\n");
        return;
    }
    for (int i = 0; i < (lst->len - 1); i++) {
        printData(&this->d);
        printf(", ");
        this = this->next;
    }
    printData(&this->d);
    putchar('\n');
}

void printLstWithNum(struct CircularLinkedList *lst, int num) {
    struct Node *this = lst->head;
    if (lst->len == 0) {
        printf("NULL\n");
        return;
    }
    for (int i = 0; i < (num - 1); i++) {
        printData(&this->d);
        printf(", ");
        this = this->next;
    }
    printData(&this->d);
    putchar('\n');
}

void insert(struct CircularLinkedList *lst, struct Node *target,
            struct Data data) {
    struct Node *newNode = (struct Node *)_malloc(sizeof(struct Node));
    newNode->d = data;
    newNode->next = target->next;
    target->next = newNode;
    lst->len++;
}

void insertFirst(struct CircularLinkedList *lst, struct Data data) {
    struct Node *newHead = (struct Node *)_malloc(sizeof(struct Node));
    struct Node *head = lst->head;
    struct Node *current = head;
    while (current->next != head) {
        current = current->next;
    }

    current->next = newHead;

    newHead->d = data;
    newHead->next = lst->head;
    lst->head = newHead;
    lst->len++;
}

int insertIndex(struct CircularLinkedList *lst, int index, struct Data data) {
    if (index < 0) {
        return -1;
    }

    if (index == 0) {
        insertFirst(lst, data);
    } else {
        struct Node *current = lst->head;
        for (int now = 0; now < index - 1; now++) {
            current = current->next;
        }
        insert(lst, current, data);
    }
    return 0;
}

int deleteIndex(struct CircularLinkedList *lst, int index) {
    if (index < 0) {
        return -1;
    }

    struct Node *current = lst->head;
    for (int now = 0; now < index; now++) {
        current = current->next;
    }

    deleteNode(lst, current);
    return 0;
}

struct Node *searchNode(struct CircularLinkedList *lst, struct Data data) {
    struct Node *this = lst->head;
    for (int i = 0; i < lst->len; i++) {
        if (dataEquals(&this->d, &data) == 1) {
            return this;
        }
        this = this->next;
    }
    return NULL;
}

struct Node *searchNodeBeginAt(struct CircularLinkedList *lst,
                               struct Node *begin, struct Data data) {
    struct Node *this = begin;
    for (int i = 0; i < lst->len; i++) {
        if (dataEquals(&this->d, &data) == 1) {
            return this;
        }
        this = this->next;
    }
    return NULL;
}

void deleteNode(struct CircularLinkedList *lst, struct Node *node) {
    if (node == lst->head) {
        lst->head = node->next;
        struct Node *this = node;
        while (this->next != NULL) {
            if (this->next == node) {
                this->next = node->next;
                _free(node);
                lst->len--;
                return;
            } else {
                this = this->next;
            }
        }
    } else {
        struct Node *this = lst->head;
        while (this->next != NULL) {
            if (this->next == node) {
                this->next = node->next;
                _free(node);
                lst->len--;
                return;
            } else {
                this = this->next;
            }
        }
    }
}

struct Node *getIndex(struct CircularLinkedList *lst, int index) {
    if (index < 0) {
        return NULL;
    }

    struct Node *current = lst->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

struct Node *seekNode(struct Node *source, int step) {
    struct Node *result = source;
    for (int i = 0; i < step; i++) {
        result = result->next;
    }
    return result;
}