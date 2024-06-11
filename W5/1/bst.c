#include "AbstractBinaryTree.h"
#include "BinarySearchTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void f(struct BinaryTree_Node *node);

int main(void) {
    int n = 0;
    scanf("%d", &n);

    struct BinaryTree *tree = BinaryTree_createTree();

    for (int i = 0; i < n; i++) {
        int input = 0;
        scanf("%d", &input);
        BinaryTree_insert(tree, &input, true);
    }

    BinaryTree_forEach(tree, 0, f);

    BinaryTree_destroyTree(tree);

    return 0;
}

void f(struct BinaryTree_Node *node) {
    if ((node->left == NULL) && (node->right == NULL)) {
        printf("%d %zu\n", *node->data,
               BinaryTree_getNodeDistanceFromRoot(node));
    }
}

BinaryTree_Data *BinaryTree_copyData(BinaryTree_Data *source) {
    BinaryTree_Data *result =
        (BinaryTree_Data *)malloc(sizeof(BinaryTree_Data));
    *result = *source;
    return result;
}
void BinaryTree_destroyData(BinaryTree_Data *target) { free(target); }

void BinaryTree_printData(BinaryTree_Data *target) {}
inline int BinaryTree_dataCmp(BinaryTree_Data *a, BinaryTree_Data *b) {
    if (*a < *b) {
        return -1;
    } else if (*a > *b) {
        return 1;
    } else {
        return 0;
    }
}