#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 105

struct Point *readPoint();
struct Line *buildLine(struct Point *p1, struct Point *p2);
struct LineNode *inLine(struct Line *l, struct Point *p);
void extendLine(struct Line *l, struct LineNode *ln, struct Point *nextPoint);
void combine(struct Line **lst, int len);
void printLine(struct Line *target);
void mergeLine(struct Line *prev, struct Line *next);
struct Point {
    int x;
    int y;
};

struct LineNode {
    struct Point *p;
    struct LineNode *next;
};

struct Line {
    struct LineNode *head;
    struct LineNode *tail;
    int len;
};

int main(void) {
    int n = 0;
    scanf("%d", &n);
    struct Line **lineLst =
        (struct Line **)malloc((unsigned)MAXLEN * sizeof(struct Line *));

    int lineLstLen = 0;

    for (int i = 0; i < n; i++) {
        struct Point *p1 = readPoint();
        struct Point *p2 = readPoint();

        int extended = 0;
        int beginCanConnect = 0;
        int endCanConnect = 0;
        struct Line *beginLine = NULL;
        struct LineNode *beginBase = NULL;
        struct Line *endLine = NULL;
        struct LineNode *endBase = NULL;
        for (int j = 0; j < lineLstLen; j++) {
            if (lineLst[j]->len != 0) {
                struct LineNode *temp = NULL;
                if ((temp = inLine(lineLst[j], p1)) != NULL) {
                    beginBase = temp;
                    beginLine = lineLst[j];
                    beginCanConnect = 1;
                } else if ((temp = inLine(lineLst[j], p2)) != NULL) {
                    endBase = temp;
                    endLine = lineLst[j];
                    endCanConnect = 1;
                }
            }
        }

        if (beginCanConnect && endCanConnect) {
            mergeLine(beginLine, endLine);
        } else if (beginCanConnect) {
            struct LineNode *next =
                (struct LineNode *)malloc(sizeof(struct LineNode));
            next->p = p2;
            next->next = NULL;
            beginBase->next = next;
            beginLine->tail = next;
            beginLine->len++;
        } else if (endCanConnect) {
            struct LineNode *prev =
                (struct LineNode *)malloc(sizeof(struct LineNode));
            prev->p = p1;
            prev->next = endLine->head;
            endLine->head = prev;
            endLine->len++;
        } else {
            lineLst[lineLstLen] = buildLine(p1, p2);
            lineLstLen++;
        }
    }

    struct Line *maxItem = lineLst[0];
    int maxCount = 0;
    for (int i = 0; i < lineLstLen; i++) {
        struct Line *l = lineLst[i];
        if ((l != NULL) && (l->len != 0)) {
            if (l->len > maxCount) {
                maxCount = l->len;
                maxItem = l;
            }
        }
    }

    printf("%d %d %d", maxCount - 1, maxItem->head->p->x, maxItem->head->p->y);

    return 0;
}

struct Point *readPoint() {
    int x = 0;
    int y = 0;
    scanf("%d%d", &x, &y);
    struct Point *result = (struct Point *)malloc(sizeof(struct Point));
    result->x = x;
    result->y = y;
    return result;
}

struct Line *buildLine(struct Point *p1, struct Point *p2) {
    struct Line *result = (struct Line *)malloc(sizeof(struct Line));
    struct LineNode *head = (struct LineNode *)malloc(sizeof(struct LineNode));
    struct LineNode *tail = (struct LineNode *)malloc(sizeof(struct LineNode));
    head->p = p1;
    tail->p = p2;
    head->next = tail;
    tail->next = NULL;
    result->head = head;
    result->tail = tail;
    result->len = 2;
    return result;
}

struct LineNode *inLine(struct Line *l, struct Point *p) {
    if ((l->head->p->x == p->x) && (l->head->p->y == p->y)) {
        return l->head;
    } else if ((l->tail->p->x == p->x) && (l->tail->p->y == p->y)) {
        return l->tail;
    }
    return NULL;
}

void extendLine(struct Line *l, struct LineNode *ln, struct Point *nextPoint) {
    if (ln == l->head) {
        struct LineNode *newHead =
            (struct LineNode *)malloc(sizeof(struct LineNode));
        newHead->p = nextPoint;
        newHead->next = l->head;
        l->head = newHead;
        l->len++;
    } else {
        struct LineNode *newTail =
            (struct LineNode *)malloc(sizeof(struct LineNode));
        newTail->p = nextPoint;
        newTail->next = NULL;
        l->tail->next = newTail;
        l->tail = newTail;
        l->len++;
    }
}

void combine(struct Line **lst, int len) {
    while (1) {
        int count = 0;
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                if ((lst[i] != NULL) && (lst[j] != NULL) && (i != j)) {
                    struct Line *first = lst[i];
                    struct Line *second = lst[j];

                    if ((first->tail->p->x == second->head->p->x) &&
                        (first->tail->p->y == second->head->p->y)) {
                        first->tail->next = second->head;
                        first->len += second->len - 1;
                        lst[j] = NULL;
                        count++;
                    }
                }
            }
        }
        if (count == 0) {
            break;
        }
    }
}

void printLine(struct Line *target) {
    if (target->len == 0) {
        printf("NULL\n");
        return;
    }
    struct LineNode *current = target->head;
    for (int i = 0; i < (target->len - 1); i++) {
        printf("{x = %d, y = %d}->", current->p->x, current->p->y);
        current = current->next;
    }
    printf("{x = %d, y = %d}\n", current->p->x, current->p->y);
}

void mergeLine(struct Line *prev, struct Line *next) {
    prev->tail->next = next->head;
    prev->tail = next->tail;
    prev->len += next->len;
    next->len = 0;
}