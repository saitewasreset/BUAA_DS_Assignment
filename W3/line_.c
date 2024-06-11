#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Line {
    uint64_t begin;
    uint64_t end;
};

struct Point {
    int x;
    int y;
};

uint64_t readPoint();
uint64_t pointEncode(struct Point p);
struct Point pointDecode(uint64_t encoded);
int beginCmp(const void *a, const void *b);
int endCmp(const void *a, const void *b);
void printPoint(uint64_t encoded);

int main(void) {
    int n = 0;
    scanf("%d", &n);
    struct Line **line =
        (struct Line **)malloc((unsigned)n * sizeof(struct Line *));

    for (int i = 0; i < n; i++) {
        uint64_t begin = readPoint();
        uint64_t end = readPoint();
        struct Line *thisLine = (struct Line *)malloc(sizeof(struct Line));
        thisLine->begin = begin;
        thisLine->end = end;
        line[i] = thisLine;
    }

    int maxLen = 1;
    uint64_t maxBegin = 0;

    for (int i = 0; i < n; i++) {
        uint64_t currentBegin = line[i]->begin;
        uint64_t currentEnd = line[i]->end;
        int lineLen = 1;
        if (i == 2) {
            printf("Fuck you");
        }
        for (int k = 0; k < n; k++) {

            for (int j = 0; j < n; j++) {
                if (j == i) {
                    continue;
                }
                if (currentEnd == line[j]->begin) {
                    currentEnd = line[j]->end;
                    lineLen++;
                } else if (currentEnd == line[j]->end) {
                    currentEnd = line[j]->begin;
                    lineLen++;
                } else if (currentBegin == line[j]->begin) {
                    currentBegin = line[j]->end;
                    lineLen++;
                } else if (currentBegin == line[j]->end) {
                    currentBegin = line[j]->begin;
                    lineLen++;
                }
            }
        }
        printf("len = %d\n", lineLen);
        if (lineLen > maxLen) {
            maxLen = lineLen;
            maxBegin = currentBegin;
        }
    }
    printf("%d ", maxLen);
    printPoint(maxBegin);
    for (int i = 0; i < n; i++) {
        free(line[i]);
    }
    free(line);
    return 0;
}

uint64_t pointEncode(struct Point p) {
    return (((uint64_t)(uint32_t)p.x) << 32) | ((uint64_t)(uint32_t)p.y);
}

struct Point pointDecode(uint64_t encoded) {
    int x = (int)(encoded >> 32);
    int y = (int)(encoded & 0xFFFFFFFF);
    struct Point result = {x, y};
    return result;
}

void printPoint(uint64_t encoded) {
    struct Point r = pointDecode(encoded);
    printf("%d %d\n", r.x, r.y);
}

uint64_t readPoint() {
    int x = 0;
    int y = 0;
    scanf("%d%d", &x, &y);
    return (((uint64_t)(uint32_t)x) << 32) | ((uint64_t)(uint32_t)y);
}

int beginCmp(const void *a, const void *b) {
    struct Line *lA = *((struct Line **)a);
    struct Line *lB = *((struct Line **)b);

    if (lA->begin < lB->begin) {
        return -1;
    } else if (lA->begin > lB->begin) {
        return 1;
    } else {
        return 0;
    }
}

int endCmp(const void *a, const void *b) {
    struct Line *lA = *((struct Line **)a);
    struct Line *lB = *((struct Line **)b);

    if (lA->end < lB->end) {
        return -1;
    } else if (lA->end > lB->end) {
        return 1;
    } else {
        return 0;
    }
}