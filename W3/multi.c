#include <stdio.h>
#include <stdlib.h>

struct PolyNode {
    int c;
    int p;
};

struct PolyList {
    struct PolyNode *node;
    struct PolyList *next;
};

int main(void) { return 0; }

struct PolyList *buildPoly() {
    struct PolyList *head = (struct PolyList *)malloc(sizeof(struct PolyList));
    head->next = NULL;
    struct PolyList *current = head;
    int c = 0;
    int p = 0;
    scanf("%d%d", &c, &p);
    struct PolyNode *node = (struct PolyNode *)malloc(sizeof(struct PolyNode));
    current->node = node;

    while (scanf("%d%d", &c, &p) == 2) {
        struct PolyNode *node =
            (struct PolyNode *)malloc(sizeof(struct PolyNode));
        node->c = c;
        node->p = p;
        current->next = (struct PolyList *)malloc(sizeof(struct PolyList));
        current->next->next = NULL;
        current->next->node = node;
        current = current->next;
    }
}