#ifndef __BIT_FILE_H
#define __BIT_FILE_H

#include <stdint.h>
#include <stdio.h>

struct BitFile {
    FILE *inner;
    uint8_t buffer;
    uint8_t bufferLen;
    size_t bitPos;
};

struct BitFile *BitFile_create(FILE *file);
void BitFile_flush(struct BitFile *bfile);
void BitFile_close(struct BitFile *bfile);

size_t BitFile_readBits(uint8_t *buffer, size_t len, struct BitFile *bfile);
size_t BitFile_writeBits(uint8_t *buffer, size_t len, struct BitFile *bfile);

// Only support reading/writing when bitPos % 8 == 0
size_t BitFile_readBytes(uint8_t *buffer, size_t len, struct BitFile *bfile);
size_t BitFile_writeBytes(uint8_t *buffer, size_t len, struct BitFile *bfile);

void BitFile_padding(struct BitFile *bfile);
void BitFile_seek(struct BitFile *bfile, size_t bitPos);

#endif
