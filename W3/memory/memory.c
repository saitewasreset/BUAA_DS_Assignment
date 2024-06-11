#include "alloc.h"

struct Data **getData(int len);
void destroyData(struct Data **target, int len);
int dataCmp(const void *a, const void *b);

int main(void) {
    int len = 0;
    scanf("%d", &len);
    struct Data **dataLst = getData(len);
    qsort(dataLst, (unsigned)len, sizeof(struct Data *), dataCmp);
    struct Alloc *memory = createAlloc(dataLst, len);

    int requestSize = 0;
    while (scanf("%d", &requestSize) != EOF) {
        if (requestSize == -1) {
            break;
        }
        alloc(memory, requestSize);
        // printAlloc(memory);
    }

    printAlloc(memory);

    destroyData(dataLst, len);
    destroyAlloc(memory);
    return 0;
}

struct Data **getData(int len) {
    int address, size;
    address = size = 0;
    struct Data **result =
        (struct Data **)malloc((unsigned)len * sizeof(struct Data *));

    for (int i = 0; i < len; i++) {
        scanf("%d%d", &address, &size);
        result[i] = (struct Data *)malloc(sizeof(struct Data));
        result[i]->address = address;
        result[i]->size = size;
    }
    return result;
}

void destroyData(struct Data **target, int len) {
    for (int i = 0; i < len; i++) {
        free(target[i]);
    }
    free(target);
}

int dataCmp(const void *a, const void *b) {
    struct Data *dA = *((struct Data **)a);
    struct Data *dB = *((struct Data **)b);
    if (dA->address < dB->address) {
        return -1;
    } else if (dA->address > dB->address) {
        return 1;
    } else {
        return 0;
    }
}