#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NAMELEN 25

struct StudentInfo {
    size_t id;
    char name[NAMELEN];
    size_t seatId;
};

int firstCmp(const void *a, const void *b);
int secondCmp(const void *a, const void *b);
struct StudentInfo **readList(size_t studentCount);

int main(void) {
    size_t studentCount = 0;
    scanf("%zu", &studentCount);

    struct StudentInfo **info = readList(studentCount);

    qsort(info, studentCount, sizeof(struct StudentInfo *), firstCmp);

    size_t maxSeatId = info[studentCount - 1]->seatId;

    size_t Q = (maxSeatId > studentCount) ? studentCount : maxSeatId;
    size_t studentIdxToSet = studentCount - 1;

    for (size_t i = 1; i <= Q; i++) {
        maxSeatId = 0;
        for (size_t j = 0; j < studentCount; j++) {
            if (info[j]->seatId > maxSeatId) {
                maxSeatId = info[j]->seatId;
            }
        }
        Q = (maxSeatId > studentCount) ? studentCount : maxSeatId;

        bool currentSet = false;
        for (size_t j = 0; j < studentCount; j++) {
            if (info[j]->seatId == i) {
                currentSet = true;
                break;
            }
        }

        if (!currentSet) {
            info[studentIdxToSet]->seatId = i;
            studentIdxToSet--;
        }
    }

    qsort(info, studentCount, sizeof(struct StudentInfo *), firstCmp);

    maxSeatId = info[studentCount - 1]->seatId;

    size_t lastSeatId = info[0]->seatId;
    for (size_t i = 1; i < studentCount; i++) {
        if (info[i]->seatId == lastSeatId) {
            maxSeatId++;
            info[i]->seatId = maxSeatId;
        }
        lastSeatId = info[i]->seatId;
    }

    qsort(info, studentCount, sizeof(struct StudentInfo *), secondCmp);

    FILE *out = fopen("out.txt", "w");

    for (size_t i = 0; i < studentCount; i++) {
        fprintf(out, "%zu %s %zu\n", info[i]->id, info[i]->name,
                info[i]->seatId);
    }

    fclose(out);
    for (size_t i = 0; i < studentCount; i++) {
        free(info[i]);
    }
    free(info);
    return 0;
}

struct StudentInfo **readList(size_t studentCount) {
    FILE *in = fopen("in.txt", "r");
    struct StudentInfo **result = (struct StudentInfo **)malloc(
        studentCount * sizeof(struct StudentInfo *));

    for (size_t i = 0; i < studentCount; i++) {
        struct StudentInfo *current =
            (struct StudentInfo *)malloc(sizeof(struct StudentInfo));
        fscanf(in, "%zu%s%zu", &current->id, current->name, &current->seatId);
        result[i] = current;
    }

    fclose(in);

    return result;
}

int firstCmp(const void *a, const void *b) {
    struct StudentInfo *sA = *((struct StudentInfo **)a);
    struct StudentInfo *sB = *((struct StudentInfo **)b);

    if (sA->seatId < sB->seatId) {
        return -1;
    } else if (sA->seatId > sB->seatId) {
        return 1;
    } else {
        if (sA->id < sB->id) {
            return -1;
        } else if (sA->id > sB->id) {
            return 1;
        } else {
            return 0;
        }
    }
}

int secondCmp(const void *a, const void *b) {
    struct StudentInfo *sA = *((struct StudentInfo **)a);
    struct StudentInfo *sB = *((struct StudentInfo **)b);

    if (sA->id < sB->id) {
        return -1;
    } else if (sA->id > sB->id) {
        return 1;
    } else {
        return 0;
    }
}