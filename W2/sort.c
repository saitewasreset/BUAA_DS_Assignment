/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 80
#define MAXNAME 30
#define MAXCOUNT 105

struct info {
    char *name;
    long long phone;
    int valid;
    int id;
};

char *s_gets(char *dst, int n);
int get(struct info **lst);
int sortFirst(const void *a, const void *b);
int sortSecond(const void *a, const void *b);
void freeInfo(struct info **target, int len);
void markAndRename(struct info **lst, int len);

int main(void) {
    int len = 0;
    scanf("%d", &len);
    getchar();
    struct info **lst =
        (struct info **)malloc((unsigned)len * sizeof(struct info *));

    get(lst);

    qsort(lst, (unsigned)len, sizeof(struct info *), sortFirst);

    markAndRename(lst, len);

    for (int i = 0; i < (len - 1); i++) {
        if (lst[i]->valid == 1) {
            printf("%s %lld\n", lst[i]->name, lst[i]->phone);
        }
    }

    if (lst[len - 1]->valid == 1) {
        printf("%s %lld", lst[len - 1]->name, lst[len - 1]->phone);
    }

    freeInfo(lst, len);

    return 0;
}

int get(struct info **lst) {
    char line[MAXLEN] = {0};
    int i = 0;
    while (s_gets(line, MAXLEN) != NULL) {
        char name[MAXNAME];
        long long phone = 0;
        sscanf(line, "%s %lld", name, &phone);
        int name_len = (int)strlen(name);

        struct info *thisInfo = (struct info *)malloc(sizeof(struct info));
        thisInfo->name =
            (char *)malloc((unsigned)(name_len + 3) * sizeof(char));
        strcpy(thisInfo->name, name);
        thisInfo->phone = phone;
        thisInfo->valid = 1;
        thisInfo->id = i;
        lst[i] = thisInfo;
        i++;
    }
    return i;
}

char *s_gets(char *dst, int n) {
    char *ret = fgets(dst, n, stdin);
    int i = 0;
    if (ret != NULL) {
        while (dst[i] != '\n' && dst[i] != '\0') {
            i++;
        }
        if (dst[i] == '\n') {
            dst[i] = '\0';
        } else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                continue;
            }
        }
    }

    return ret;
}

void markAndRename(struct info **lst, int len) {
    if (len == 1) {
        return;
    }
    char *thisName = lst[0]->name;
    long long thisPhone = lst[0]->phone;
    for (int i = 1; i < len; i++) {
        if (strcmp(lst[i]->name, thisName) == 0) {
            if (lst[i]->phone == thisPhone) {
                lst[i]->valid = 0;
            }
        } else {
            thisName = lst[i]->name;
            thisPhone = lst[i]->phone;
        }
    }

    qsort(lst, (unsigned)len, sizeof(struct info *), sortSecond);

    thisName = (char *)malloc((unsigned)MAXLEN * sizeof(char));
    strcpy(thisName, lst[0]->name);
    int repetition_count = 0;
    for (int i = 1; i < len; i++) {
        if (strcmp(lst[i]->name, thisName) == 0) {
            repetition_count++;
            char temp[MAXLEN] = {0};
            sprintf(temp, "%s_%d", lst[i]->name, repetition_count);
            strcpy(lst[i]->name, temp);
        } else {
            strcpy(thisName, lst[i]->name);
            repetition_count = 0;
        }
    }
    free(thisName);
}

int sortFirst(const void *a, const void *b) {
    const struct info *iA = *((const struct info **)a);
    const struct info *iB = *((const struct info **)b);
    int r = strcmp(iA->name, iB->name);
    if (r < 0) {
        return -1;
    } else if (r > 0) {
        return 1;
    } else {
        if (iA->phone < iB->phone) {
            return -1;
        } else if (iA->phone > iB->phone) {
            return 1;
        } else {
            return 0;
        }
    }
}

int sortSecond(const void *a, const void *b) {
    const struct info *iA = *((const struct info **)a);
    const struct info *iB = *((const struct info **)b);
    int r = strcmp(iA->name, iB->name);
    if (r < 0) {
        return -1;
    } else if (r > 0) {
        return 1;
    } else {
        if (iA->id < iB->id) {
            return -1;
        } else if (iA->id > iB->id) {
            return 1;
        } else {
            return 0;
        }
    }
}

void freeInfo(struct info **target, int len) {
    for (int i = 0; i < len; i++) {
        free(target[i]->name);
        free(target[i]);
    }
    free(target);
}