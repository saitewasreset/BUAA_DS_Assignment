#include "libhzip.h"
#include "BitFile.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct LinkedListNode {
    struct HuffmanTreeNode *treeNode;
    struct LinkedListNode *next;
};

struct LinkedListNode *insertOrdered(struct LinkedListNode *head,
                                     struct HuffmanTreeNode *data);

void destroyHuffmanTree(struct HuffmanTreeNode *target);

void writeFileHeader(struct BitFile *dst, struct HuffmanMapEntry (*map)[128]);
void writeFileContent(FILE *src, struct BitFile *dst,
                      struct HuffmanMapEntry (*map)[128]);

static uint8_t currentCode[MAX_CODE_LENGTH] = {0};
void preOrder(struct HuffmanTreeNode *node, uint8_t currentCodeLen,
              struct HuffmanMapEntry (*map)[128]);

struct LinkedListNode *insertOrdered(struct LinkedListNode *head,
                                     struct HuffmanTreeNode *data) {
    struct LinkedListNode *newNode =
        (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
    newNode->treeNode = data;

    struct LinkedListNode *current = head;

    if (current->treeNode->weight > data->weight) {
        newNode->next = head;
        return newNode;
    }

    if ((current->treeNode->weight == data->weight) &&
        (current->treeNode->c > data->c) &&
        ((data->left == NULL) && (data->right == NULL))) {
        newNode->next = head;
        return newNode;
    }

    while ((current->next != NULL) &&
           (current->next->treeNode->weight < data->weight)) {
        current = current->next;
    }

    if (current->next == NULL) {
        current->next = newNode;
        newNode->next = NULL;
        return head;
    } else if (current->next->treeNode->weight > data->weight) {
        newNode->next = current->next;
        current->next = newNode;
        return head;
    } else {
        if ((data->left == NULL) && (data->right == NULL)) {
            while ((current->next != NULL) &&
                   (current->next->treeNode->c < data->c) &&
                   (current->next->treeNode->weight == data->weight)) {
                current = current->next;
            }

            if (current->next == NULL) {
                current->next = newNode;
                newNode->next = NULL;
                return head;
            } else {
                newNode->next = current->next;
                current->next = newNode;
                return head;
            }
        } else {
            while ((current->next != NULL) &&
                   (current->next->treeNode->weight == data->weight)) {
                current = current->next;
            }

            if (current->next == NULL) {
                current->next = newNode;
                newNode->next = NULL;
                return head;
            } else {
                newNode->next = current->next;
                current->next = newNode;
                return head;
            }
        }
    }
}

struct HuffmanTreeNode *generateHuffmanTree(FILE *src) {
    size_t charCount[128] = {0};

    // use '\0' as end of file flag
    charCount['\0'] = 1;

    int ch = 0;

    while ((ch = fgetc(src)) != EOF) {
        charCount[ch]++;
    }

    struct HuffmanTreeNode *first =
        (struct HuffmanTreeNode *)malloc(sizeof(struct HuffmanTreeNode));
    first->c = '\0';
    first->weight = 1;
    first->left = NULL;
    first->right = NULL;

    struct LinkedListNode *head =
        (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
    head->treeNode = first;
    head->next = NULL;

    for (int i = 1; i < 128; i++) {
        if (charCount[i] > 0) {
            struct HuffmanTreeNode *current = (struct HuffmanTreeNode *)malloc(
                sizeof(struct HuffmanTreeNode));
            current->c = (uint8_t)i;
            current->weight = charCount[i];
            current->left = NULL;
            current->right = NULL;
            head = insertOrdered(head, current);
        }
    }

    uint8_t nodeId = 0;

    while (head->next->next != NULL) {
        struct HuffmanTreeNode *parent =
            (struct HuffmanTreeNode *)malloc(sizeof(struct HuffmanTreeNode));
        parent->left = head->treeNode;
        parent->right = head->next->treeNode;
        parent->weight = parent->left->weight + parent->right->weight;
        parent->c = nodeId;
        nodeId++;
        struct LinkedListNode *newHead = head->next->next;

        free(head->next);
        free(head);

        head = newHead;

        head = insertOrdered(head, parent);
    }

    struct HuffmanTreeNode *final =
        (struct HuffmanTreeNode *)malloc(sizeof(struct HuffmanTreeNode));
    final->left = head->treeNode;
    final->right = head->next->treeNode;
    final->weight = final->left->weight + final->right->weight;
    final->c = nodeId;

    free(head->next);
    free(head);

    return final;
}

void destroyHuffmanTree(struct HuffmanTreeNode *target) {
    if (target == NULL) {
        return;
    }

    destroyHuffmanTree(target->left);
    destroyHuffmanTree(target->right);
    free(target);
}

void generateHuffmanCodeMap(struct HuffmanTreeNode *tree,
                            struct HuffmanMapEntry (*map)[128]) {
    preOrder(tree, 0, map);
}
void preOrder(struct HuffmanTreeNode *node, uint8_t currentCodeLen,
              struct HuffmanMapEntry (*map)[128]) {
    if (node == NULL) {
        return;
    }

    if ((node->left == NULL) && (node->right == NULL)) {
        (*map)[(node->c)].codeLength = currentCodeLen;
        memcpy((*map)[(node->c)].code, currentCode,
               currentCodeLen * sizeof(uint8_t));
        return;
    }

    currentCode[currentCodeLen] = 0;
    preOrder(node->left, currentCodeLen + 1, map);

    currentCode[currentCodeLen] = 1;
    preOrder(node->right, currentCodeLen + 1, map);
}

void compress(FILE *src, FILE *dst) {
    fseek(src, 0, SEEK_SET);
    fseek(dst, 0, SEEK_SET);

    struct HuffmanTreeNode *tree = generateHuffmanTree(src);

    struct HuffmanMapEntry map[128] = {0};

    generateHuffmanCodeMap(tree, &map);
    printHuffmanTreeCodeMap(&map);
    struct BitFile *bfDst = BitFile_create(dst);

    writeFileHeader(bfDst, &map);
    writeFileContent(src, bfDst, &map);

    BitFile_close(bfDst);

    destroyHuffmanTree(tree);
}

void writeFileHeader(struct BitFile *dst, struct HuffmanMapEntry (*map)[128]) {
    // Entry count - 1byte
    uint8_t entryCount = 0;
    for (size_t i = 0; i < 128; i++) {
        if ((*map)[i].codeLength > 0) {
            entryCount++;
        }
    }

    BitFile_writeBytes(&entryCount, sizeof(uint8_t), dst);

    for (size_t i = 0; i < 128; i++) {
        if ((*map)[i].codeLength > 0) {

            // ASCII - 1byte

            BitFile_writeBytes((uint8_t *)&i, sizeof(uint8_t), dst);

            // code Length - 1byte
            BitFile_writeBytes(&(*map)[i].codeLength, sizeof(uint8_t), dst);

            // Huffman code >= 1byte
            BitFile_writeBits((*map)[i].code, (*map)[i].codeLength, dst);

            BitFile_flush(dst);
        }
    }
}

void writeFileContent(FILE *src, struct BitFile *dst,
                      struct HuffmanMapEntry (*map)[128]) {
    fseek(src, 0, SEEK_SET);

    int ch = 0;
    while ((ch = fgetc(src)) != EOF) {

        uint8_t *code = (*map)[ch].code;

        BitFile_writeBits(code, (*map)[ch].codeLength, dst);
    }

    uint8_t *code = (*map)[0].code;

    BitFile_writeBits(code, (*map)[0].codeLength, dst);
}

void decompress(FILE *src, FILE *dst) {
    fseek(src, 0, SEEK_END);
    size_t fileEndPos = (size_t)ftell(src);
    fseek(src, 0, SEEK_SET);
    fseek(dst, 0, SEEK_SET);

    struct HuffmanMapEntry map[128] = {0};

    struct BitFile *srcBf = BitFile_create(src);

    readHuffmanCodeMap(srcBf, &map);

    struct HuffmanTreeNode *tree = generateHuffmanTreeFromCodeMap(&map);

    uint8_t currentBit = 0;
    struct HuffmanTreeNode *current = tree;

    size_t remainBytes = fileEndPos - (size_t)ftell(srcBf->inner);
    size_t remainBits = remainBytes * 8;

    size_t bufferSize = (remainBits < BUFFER_SIZE) ? remainBits : BUFFER_SIZE;
    uint8_t *bitBuffer = (uint8_t *)malloc(bufferSize * sizeof(uint8_t));

    size_t processed = 0;

    while (processed < remainBits) {
        size_t toReadBitCount = (remainBits - processed > bufferSize)
                                    ? bufferSize
                                    : (remainBits - processed);
        size_t actualBits = BitFile_readBits(bitBuffer, toReadBitCount, srcBf);
        processed += actualBits;

        for (size_t i = 0; i < actualBits; i++) {
            currentBit = bitBuffer[i];
            if (currentBit == 0) {
                current = current->left;

            } else {
                current = current->right;
            }

            if ((current->left == NULL) && (current->right == NULL)) {
                if (current->c != 0) {
                    fputc(current->c, dst);
                } else {
                    break;
                }

                current = tree;
            }
        }
    }

    BitFile_close(srcBf);
    destroyHuffmanTree(tree);
}

void readHuffmanCodeMap(struct BitFile *src,
                        struct HuffmanMapEntry (*map)[128]) {
    uint8_t entryLength = 0;
    BitFile_readBytes(&entryLength, sizeof(uint8_t), src);

    for (uint8_t i = 0; i < entryLength; i++) {
        uint8_t ascii = 0;
        uint8_t codeLength = 0;
        BitFile_readBytes(&ascii, sizeof(uint8_t), src);
        BitFile_readBytes(&codeLength, sizeof(uint8_t), src);

        (*map)[ascii].codeLength = codeLength;
        BitFile_readBits((*map)[ascii].code, codeLength, src);

        if (src->bitPos % 8 != 0) {
            BitFile_seek(src, 8 * (src->bitPos / 8 + 1));
        }
    }
}

struct HuffmanTreeNode *
generateHuffmanTreeFromCodeMap(struct HuffmanMapEntry (*map)[128]) {
    struct HuffmanTreeNode *root =
        (struct HuffmanTreeNode *)malloc(sizeof(struct HuffmanTreeNode));
    root->left = NULL;
    root->right = NULL;
    root->c = '\0';
    root->weight = 0;
    for (uint8_t i = 0; i < 128; i++) {
        if ((*map)[i].codeLength != 0) {
            struct HuffmanTreeNode *current = root;

            for (uint8_t j = 0; j < (*map)[i].codeLength; j++) {
                uint8_t currentDirection = (*map)[i].code[j];

                if (currentDirection == 0) {
                    if (current->left == NULL) {
                        current->left = (struct HuffmanTreeNode *)malloc(
                            sizeof(struct HuffmanTreeNode));
                        current->left->left = NULL;
                        current->left->right = NULL;
                        current->left->c = '\0';
                        current->left->weight = 0;
                    }
                    current = current->left;
                } else {
                    if (current->right == NULL) {
                        current->right = (struct HuffmanTreeNode *)malloc(
                            sizeof(struct HuffmanTreeNode));
                        current->right->left = NULL;
                        current->right->right = NULL;
                        current->right->c = '\0';
                        current->right->weight = 0;
                    }
                    current = current->right;
                }
            }

            current->c = i;
        }
    }

    return root;
}

void printHuffmanTreeCodeMap(struct HuffmanMapEntry (*map)[128]) {
    for (int i = 0; i < 128; i++) {
        if ((*map)[i].codeLength != 0) {
            printf("%c:", i);
            for (int j = 0; j < (*map)[i].codeLength; j++) {
                printf("%d", (*map)[i].code[j]);
            }
            putchar('\n');
        }
    }
}
