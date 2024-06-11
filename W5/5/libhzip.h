#ifndef __LIB_HZIP_H
#define __LIB_HZIP_H

#include "BitFile.h"
#include <stdint.h>
#include <stdio.h>

#define TREE_MAX_HEIGHT 1024
#define MAX_CODE_LENGTH 255

// bytes in memory, bits in operation
#define BUFFER_SIZE 64 * 1024 * 1024

struct HuffmanTreeNode {
    uint8_t c;
    size_t weight;
    struct HuffmanTreeNode *left;
    struct HuffmanTreeNode *right;
};

struct HuffmanMapEntry {
    uint8_t code[MAX_CODE_LENGTH];
    // bits
    uint8_t codeLength;
};

void compress(FILE *src, FILE *dst);
void decompress(FILE *src, FILE *dst);

struct HuffmanTreeNode *generateHuffmanTree(FILE *src);
void generateHuffmanCodeMap(struct HuffmanTreeNode *tree,
                            struct HuffmanMapEntry (*map)[128]);
void readHuffmanCodeMap(struct BitFile *src,
                        struct HuffmanMapEntry (*map)[128]);

struct HuffmanTreeNode *
    generateHuffmanTreeFromCodeMap(struct HuffmanMapEntry (*map)[128]);
void printHuffmanTreeCodeMap(struct HuffmanMapEntry (*map)[128]);
#endif