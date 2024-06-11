#include "BitFile.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitFile *BitFile_create(FILE *file) {
    struct BitFile *bfile = (struct BitFile *)malloc(sizeof(struct BitFile));
    bfile->inner = file;
    bfile->bitPos = 0;
    bfile->buffer = 0;
    bfile->bufferLen = 0;
    return bfile;
}

void BitFile_flush(struct BitFile *bfile) {
    if (bfile->bufferLen > 0) {
        BitFile_padding(bfile);
        fputc(bfile->buffer, bfile->inner);
        bfile->buffer = 0;
        bfile->bufferLen = 0;
    }
}
void BitFile_close(struct BitFile *bfile) {
    BitFile_flush(bfile);
    free(bfile);
}

size_t BitFile_readBits(uint8_t *buffer, size_t len, struct BitFile *bfile) {
    size_t bytesToRead = len / 8 + 2;

    uint8_t *bytesBuffer = (uint8_t *)malloc(bytesToRead * sizeof(uint8_t));
    size_t innerBeginPos = bfile->bitPos / 8;

    if (ftell(bfile->inner) != (long)innerBeginPos) {
        if (fseek(bfile->inner, (long)innerBeginPos, SEEK_SET) != 0) {
            return 0;
        }
    }

    size_t successBytesCount =
        fread(bytesBuffer, sizeof(uint8_t), bytesToRead, bfile->inner);

    if (successBytesCount == 0) {
        return 0;
    }

    size_t bufferIndex = 0;

    if (bfile->bitPos % 8 != 0) {
        uint8_t leadingBitsCount = (uint8_t)(8 - (bfile->bitPos % 8));

        for (uint8_t i = 0; i < leadingBitsCount; i++) {
            if (bufferIndex >= len) {
                break;
            }
            buffer[bufferIndex] =
                (bytesBuffer[0] >> (leadingBitsCount - i - 1)) & 1;
            bufferIndex++;
        }
    } else {
        uint8_t leadingBitsCount = (len >= 8) ? 8 : (uint8_t)len;

        for (uint8_t i = 0; i < leadingBitsCount; i++) {
            buffer[bufferIndex] = (bytesBuffer[0] >> (7 - i)) & 1;
            bufferIndex++;
        }
    }

    for (size_t i = 1; i < successBytesCount - 1; i++) {
        if (bufferIndex >= len) {
            break;
        }
        for (uint8_t j = 0; j < 8; j++) {
            buffer[bufferIndex] = (bytesBuffer[i] >> (7 - j)) & 1;
            bufferIndex++;
        }
    }

    if (successBytesCount == bytesToRead) {
        uint8_t endingBitsCount = (bfile->bitPos + len + 1) % 8;
        for (uint8_t i = 0; i < endingBitsCount; i++) {
            if (bufferIndex >= len) {
                break;
            }
            buffer[bufferIndex] =
                (bytesBuffer[successBytesCount - 1] >> (7 - i)) & 1;
            bufferIndex++;
        }
    } else {
        if (successBytesCount > 1) {
            uint8_t endingBitsCount = (uint8_t)(len - bufferIndex);
            endingBitsCount = (endingBitsCount >= 8) ? 8 : endingBitsCount;
            for (uint8_t i = 0; i < endingBitsCount; i++) {
                if (bufferIndex >= len) {
                    break;
                }
                buffer[bufferIndex] =
                    (bytesBuffer[successBytesCount - 1] >> (7 - i)) & 1;
                bufferIndex++;
            }
        }
    }

    bfile->bitPos += bufferIndex;
    free(bytesBuffer);
    return bufferIndex;
}
size_t BitFile_writeBits(uint8_t *buffer, size_t len, struct BitFile *bfile) {
    if (ftell(bfile->inner) != (long)bfile->bitPos / 8) {
        fseek(bfile->inner, (long)bfile->bitPos / 8, SEEK_SET);
    }

    size_t bufferIndex = 0;

    if (bfile->bufferLen > 0) {
        uint8_t toBuffer = (uint8_t)(8 - bfile->bufferLen);
        if (toBuffer > len) {
            toBuffer = (uint8_t)len;
        }
        for (uint8_t i = 0; i < toBuffer; i++) {
            bfile->buffer =
                (uint8_t)((bfile->buffer << 1) | buffer[bufferIndex]);
            bufferIndex++;
            bfile->bitPos++;
            bfile->bufferLen++;
        }
        if (bfile->bufferLen == 8) {
            fputc(bfile->buffer, bfile->inner);
            bfile->buffer = 0;
            bfile->bufferLen = 0;
        } else {
            return 0;
        }
    }

    size_t remainBits = len - bufferIndex;

    if (bfile->bitPos % 8 != 0) {
        int ch = fgetc(bfile->inner);
        if (ch == EOF) {
            return 0;
        }

        uint8_t firstByte = (uint8_t)ch;
        uint8_t firstWriteBitsCount = (uint8_t)(8 - (bfile->bitPos % 8));

        for (uint8_t i = 0; i < firstWriteBitsCount; i++) {
            firstByte =
                (uint8_t)((firstByte & ~(1 << (firstWriteBitsCount - i - 1))) |
                          (buffer[bufferIndex]
                           << (firstWriteBitsCount - i - 1)));
            bufferIndex++;
        }

        if (ftell(bfile->inner) != (long)bfile->bitPos / 8) {
            fseek(bfile->inner, (long)bfile->bitPos / 8, SEEK_SET);
        }
        fputc(firstByte, bfile->inner);
    }

    uint8_t currentByte = 0;
    uint8_t currentByteLen = 0;

    while (bufferIndex < len) {
        currentByte = (uint8_t)((currentByte << 1) | buffer[bufferIndex]);
        currentByteLen++;

        if (currentByteLen == 8) {
            fputc(currentByte, bfile->inner);
            currentByteLen = 0;
        }

        bufferIndex++;
    }

    if (currentByteLen != 0) {
        bfile->bufferLen = currentByteLen;
        bfile->buffer = currentByte;
    }

    bfile->bitPos += remainBits;
    return bufferIndex;
}

size_t BitFile_readBytes(uint8_t *buffer, size_t len, struct BitFile *bfile) {
    if (ftell(bfile->inner) != (long)bfile->bitPos / 8) {
        fseek(bfile->inner, (long)bfile->bitPos / 8, SEEK_SET);
    }
    if (bfile->bitPos % 8 == 0) {
        size_t bytesRead = fread(buffer, sizeof(uint8_t), len, bfile->inner);
        bfile->bitPos += bytesRead * 8;
        return bytesRead;
    } else {
        return 0;
    }
}
size_t BitFile_writeBytes(uint8_t *buffer, size_t len, struct BitFile *bfile) {
    if (ftell(bfile->inner) != (long)bfile->bitPos / 8) {
        fseek(bfile->inner, (long)bfile->bitPos / 8, SEEK_SET);
    }
    if (bfile->bitPos % 8 == 0) {
        size_t bytesWrite = fwrite(buffer, sizeof(uint8_t), len, bfile->inner);
        bfile->bitPos += bytesWrite * 8;
        return bytesWrite;
    } else {
        return 0;
    }
}

void BitFile_padding(struct BitFile *bfile) {
    if (bfile->bitPos % 8 != 0) {
        uint8_t paddingCount = (uint8_t)(8 - bfile->bufferLen);
        bfile->buffer = (uint8_t)(bfile->buffer << paddingCount);
        bfile->bitPos += paddingCount;
    }
}

void BitFile_seek(struct BitFile *bfile, size_t bitPos) {
    bfile->bitPos = bitPos;
}
