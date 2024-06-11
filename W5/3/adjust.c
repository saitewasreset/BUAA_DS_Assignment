#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_LEN 100

struct Tree {
    size_t len;
    struct Node *root;
};

struct Node {
    int value;
    struct Node *left;
    struct Node *right;
    struct Node *down;
};

struct GateInfo {
    int id;
    int hotIndex;
};

struct Tree *createTree(void);
void _destroryTree(struct Node *node);
void destroyTree(struct Tree *tree);
void forEach(struct Tree *tree, void (*f)(struct Node *node));
struct Node *findNode(struct Tree *tree, int value);
struct Node *_findNode(struct Node *node, int value);
void readFromPattern(struct Tree *tree);
void adjust(struct Node *node);

int gateCmp(const void *a, const void *b);

struct GateInfo **infoList = NULL;
size_t infoConsumed = 0;

int main(void) {
    struct Tree *tree = createTree();
    readFromPattern(tree);

    infoList =
        (struct GateInfo **)malloc(tree->len * sizeof(struct GateInfo *));
    size_t infoListLen = 0;

    int gateId = 0;
    int gateHotIndex = 0;
    while (scanf("%d%d", &gateId, &gateHotIndex) == 2) {
        struct GateInfo *current =
            (struct GateInfo *)malloc(sizeof(struct GateInfo));
        current->id = gateId;
        current->hotIndex = gateHotIndex;

        infoList[infoListLen] = current;
        infoListLen++;
    }

    qsort(infoList, infoListLen, sizeof(struct GateInfo *), gateCmp);

    forEach(tree, adjust);

    for (size_t i = 0; i < infoListLen; i++) {
        free(infoList[i]);
    }

    free(infoList);

    destroyTree(tree);
    return 0;
}

struct Tree *createTree(void) {
    struct Tree *tree = (struct Tree *)malloc(sizeof(struct Tree));
    tree->len = 0;
    tree->root = NULL;
    return tree;
}

void destroyTree(struct Tree *tree) {
    _destroryTree(tree->root);
    free(tree);
}

void _destroryTree(struct Node *node) {
    if (node == NULL) {
        return;
    }
    _destroryTree(node->left);
    _destroryTree(node->right);
    _destroryTree(node->down);
    free(node);
}

void forEach(struct Tree *tree, void (*f)(struct Node *node)) {
    struct Node *gateQueue[MAX_QUEUE_LEN] = {0};
    struct Node *crossQueue[MAX_QUEUE_LEN] = {0};

    size_t gateQueueFront = 0;
    size_t gateQueueRear = MAX_QUEUE_LEN - 1;
    size_t gateQueueCount = 0;

    size_t crossQueueFront = 0;
    size_t crossQueueRear = MAX_QUEUE_LEN - 1;
    size_t crossQueueCount = 0;

    crossQueueRear = (crossQueueRear + 1) % MAX_QUEUE_LEN;
    crossQueue[crossQueueRear] = tree->root;
    crossQueueCount++;

    while ((gateQueueCount > 0) || (crossQueueCount > 0)) {
        while (gateQueueCount > 0) {
            struct Node *gate = gateQueue[gateQueueFront];
            gateQueueFront = (gateQueueFront + 1) % MAX_QUEUE_LEN;
            gateQueueCount--;

            f(gate);
        }

        if (crossQueueCount > 0) {
            struct Node *cross = crossQueue[crossQueueFront];
            crossQueueFront = (crossQueueFront + 1) % MAX_QUEUE_LEN;
            crossQueueCount--;

            if (cross->left != NULL) {
                if (cross->left->value < 100) {
                    gateQueueRear = (gateQueueRear + 1) % MAX_QUEUE_LEN;
                    gateQueue[gateQueueRear] = cross->left;
                    gateQueueCount++;
                } else {
                    crossQueueRear = (crossQueueRear + 1) % MAX_QUEUE_LEN;
                    crossQueue[crossQueueRear] = cross->left;
                    crossQueueCount++;
                }
            }

            if (cross->down != NULL) {
                if (cross->down->value < 100) {
                    gateQueueRear = (gateQueueRear + 1) % MAX_QUEUE_LEN;
                    gateQueue[gateQueueRear] = cross->down;
                    gateQueueCount++;
                } else {
                    crossQueueRear = (crossQueueRear + 1) % MAX_QUEUE_LEN;
                    crossQueue[crossQueueRear] = cross->down;
                    crossQueueCount++;
                }
            }

            if (cross->right != NULL) {
                if (cross->right->value < 100) {
                    gateQueueRear = (gateQueueRear + 1) % MAX_QUEUE_LEN;
                    gateQueue[gateQueueRear] = cross->right;
                    gateQueueCount++;
                } else {
                    crossQueueRear = (crossQueueRear + 1) % MAX_QUEUE_LEN;
                    crossQueue[crossQueueRear] = cross->right;
                    crossQueueCount++;
                }
            }
        }
    }
}

struct Node *findNode(struct Tree *tree, int value) {
    return _findNode(tree->root, value);
}

struct Node *_findNode(struct Node *node, int value) {
    if (node == NULL) {
        return NULL;
    }
    if (node->value == value) {
        return node;
    } else {
        struct Node *ret = NULL;
        ret = _findNode(node->left, value);
        if (ret != NULL) {
            return ret;
        }

        ret = _findNode(node->right, value);
        if (ret != NULL) {
            return ret;
        }

        ret = _findNode(node->down, value);
        return ret;
    }
}

void readFromPattern(struct Tree *tree) {
    tree->root = (struct Node *)malloc(sizeof(struct Node));
    tree->root->value = 100;
    tree->root->left = NULL;
    tree->root->right = NULL;
    tree->root->down = NULL;

    tree->len = 1;
    while (1) {
        int rootValue = 0;
        scanf("%d", &rootValue);
        if (rootValue == -1) {
            break;
        }
        struct Node *root = findNode(tree, rootValue);

        int leftValue = 0;
        scanf("%d", &leftValue);

        struct Node *left = (struct Node *)malloc(sizeof(struct Node));
        left->value = leftValue;
        left->left = NULL;
        left->right = NULL;
        left->down = NULL;

        root->left = left;
        tree->len++;

        int downValue = 0;
        scanf("%d", &downValue);
        if (downValue != -1) {
            struct Node *down = (struct Node *)malloc(sizeof(struct Node));
            down->value = downValue;
            down->left = NULL;
            down->right = NULL;
            down->down = NULL;

            root->down = down;
            tree->len++;

        } else {
            continue;
        }

        int rightValue = 0;
        scanf("%d", &rightValue);
        if (rightValue != -1) {
            struct Node *right = (struct Node *)malloc(sizeof(struct Node));
            right->value = rightValue;
            right->left = NULL;
            right->right = NULL;
            right->down = NULL;

            root->right = right;
            tree->len++;
            // ending -1
            scanf("%d", &downValue);
        } else {
            continue;
        }
    }
}

int gateCmp(const void *a, const void *b) {
    const struct GateInfo *gA = *((const struct GateInfo **)a);
    const struct GateInfo *gB = *((const struct GateInfo **)b);

    if (gA->hotIndex > gB->hotIndex) {
        return -1;
    } else if (gA->hotIndex < gB->hotIndex) {
        return 1;
    } else {
        if (gA->id < gB->id) {
            return -1;
        } else if (gA->id > gB->id) {
            return 1;
        } else {
            return 0;
        }
    }
}

void adjust(struct Node *node) {
    printf("%d->%d\n", infoList[infoConsumed]->id, node->value);
    infoConsumed++;
}