#include "LinkedList.h"

struct LinkedList *createList(struct Data initData) {
    struct LinkedList *result =
        (struct LinkedList *)_malloc(sizeof(struct LinkedList));
    result->len = 1;

    struct Node *head = (struct Node *)_malloc(sizeof(struct Node));
    head->next = NULL;

    result->head = head;
    head->d = initData;

    return result;
}

struct LinkedList *createListWithNum(int n) {
    struct LinkedList *result =
        (struct LinkedList *)_malloc(sizeof(struct LinkedList));
    result->len = n;

    struct Node *head = (struct Node *)_malloc(sizeof(struct Node));
    head->next = NULL;

    result->head = head;

    struct Node *current = head;

    for (int i = 1; i < n; i++) {
        struct Node *next = (struct Node *)_malloc(sizeof(struct Node));
        next->next = NULL;
        current->next = next;
        current = next;
    }

    return result;
}

int getLength(struct LinkedList *lst) { return lst->len; }

void destroyList(struct LinkedList *lst) {
    struct Node *this = lst->head;
    while (this->next != NULL) {
        struct Node *next = this->next;
        _free(this);
        this = next;
    }
    _free(this);
    _free(lst);
}

void printLst(struct LinkedList *lst) {
    struct Node *this = lst->head;
    while (this->next != NULL) {
        printData(&this->d);
        this = this->next;
    }
    printData(&this->d);
}

void insert(struct LinkedList *lst, struct Node *target, struct Data data) {
    struct Node *newNode = (struct Node *)_malloc(sizeof(struct Node));
    newNode->d = data;
    newNode->next = target->next;
    target->next = newNode;
    lst->len++;
}

void insertFirst(struct LinkedList *lst, struct Data data) {
    struct Node *newHead = (struct Node *)_malloc(sizeof(struct Node));
    newHead->d = data;
    newHead->next = lst->head;
    lst->head = newHead;
    lst->len++;
}

int insertIndex(struct LinkedList *lst, int index, struct Data data) {
    if ((index < 0) || (index > lst->len)) {
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

void insertOrdered(struct LinkedList *lst, struct Data data) {
    struct Node *current = lst->head;
    struct Node *new = (struct Node *)_malloc(sizeof(struct Node));
    new->d = data;
    if (dataCmp(&(current->d), &data) >= 0) {
        new->next = current;
        lst->head = new;
        lst->len++;
        return;
    }
    while (current->next != NULL) {
        if (dataCmp(&(current->next->d), &data) >= 0) {
            new->next = current->next;
            current->next = new;
            lst->len++;
            return;
        }
        current = current->next;
    }
    new->next = NULL;
    current->next = new;
    lst->len++;
    return;
}

int deleteIndex(struct LinkedList *lst, int index) {
    if ((index < 0) || (index >= lst->len)) {
        return -1;
    }

    struct Node *current = lst->head;
    for (int now = 0; now < index; now++) {
        current = current->next;
    }

    deleteNode(lst, current);
    return 0;
}

struct Node *searchNode(struct LinkedList *lst, struct Data data) {
    struct Node *this = lst->head;
    while (this->next != NULL) {
        if (dataEquals(&this->d, &data) == 1) {
            return this;
        }
        this = this->next;
    }
    if (dataEquals(&this->d, &data) == 1) {
        return this;
    } else {
        return NULL;
    }
}

struct Node *searchNodeBeginAt(struct Node *begin, struct Data data) {
    struct Node *this = begin;
    while (this->next != NULL) {
        if (dataEquals(&this->d, &data) == 1) {
            return this;
        }
        this = this->next;
    }
    if (dataEquals(&this->d, &data) == 1) {
        return this;
    } else {
        return NULL;
    }
}

void deleteNode(struct LinkedList *lst, struct Node *node) {
    if (node == lst->head) {
        lst->head = node->next;
        _free(node);
        lst->len--;
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

struct Node *getIndex(struct LinkedList *lst, int index) {
    if ((index < 0) || (index >= lst->len)) {
        return NULL;
    }

    struct Node *current = lst->head;
    for (int now = 0; now < index; now++) {
        current = current->next;
    }

    return current;
}
