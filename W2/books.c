/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN 205
#define MAXLEN 505

struct Book {
    char *name;
    char *author;
    char *press;
    char *date;
    int valid;
};

int loadFromFile(struct Book **result);
struct Book *buildFromString(char *str);
void freeLst(struct Book **target, int len);
char *s_gets(char *dst, int n);
int bookCmp(const void *a, const void *b);
void printBook(struct Book *target);
void filterAndPrint(struct Book **lst, int len, char *p);
void filterAndDelete(struct Book **lst, int len, char *p);
void save(struct Book **lst, int len);

int main(void) {
    struct Book **lst =
        (struct Book **)malloc((unsigned)MAXLEN * sizeof(struct Book *));
    int len = loadFromFile(lst);

    qsort(lst, (unsigned)len, sizeof(struct Book *), bookCmp);

    int op = 0;
    char buffer[STRLEN] = {0};
    while (1) {
        scanf("%d", &op);
        getchar();
        switch (op) {
        case 1:
            s_gets(buffer, STRLEN);
            lst[len] = buildFromString(buffer);
            len++;
            qsort(lst, (unsigned)len, sizeof(struct Book *), bookCmp);
            break;
        case 2:
            s_gets(buffer, STRLEN);
            filterAndPrint(lst, len, buffer);
            break;
        case 3:
            s_gets(buffer, STRLEN);
            filterAndDelete(lst, len, buffer);
            break;
        case 0:
            save(lst, len);
            freeLst(lst, len);
            return 0;
        }
    }
}

int loadFromFile(struct Book **result) {
    FILE *in = fopen("books.txt", "r");
    if (in == NULL) {
        printf("Cannot open books.txt");
        exit(1);
    }

    int len = 0;

    char line[STRLEN] = {0};
    while (fgets(line, STRLEN, in) != NULL) {
        struct Book *thisBook = buildFromString(line);
        result[len] = thisBook;
        len++;
    }
    return len;
}

struct Book *buildFromString(char *str) {
    char name[STRLEN] = {0};
    char author[STRLEN] = {0};
    char press[STRLEN] = {0};
    char date[STRLEN] = {0};
    sscanf(str, "%s %s %s %s", name, author, press, date);
    size_t nameLen = strlen(name);
    size_t authorLen = strlen(author);
    size_t pressLen = strlen(press);
    size_t dateLen = strlen(date);
    struct Book *result = (struct Book *)malloc(sizeof(struct Book));
    result->name = (char *)malloc((nameLen + 1) * sizeof(char));
    strcpy(result->name, name);
    result->author = (char *)malloc((authorLen + 1) * sizeof(char));
    strcpy(result->author, author);
    result->press = (char *)malloc((pressLen + 1) * sizeof(char));
    strcpy(result->press, press);
    result->date = (char *)malloc((dateLen + 1) * sizeof(char));
    strcpy(result->date, date);
    result->valid = 1;
    return result;
}

void freeLst(struct Book **target, int len) {
    for (int i = 0; i < len; i++) {
        free(target[i]->name);
        free(target[i]->author);
        free(target[i]->press);
        free(target[i]->date);
        free(target[i]);
    }
    free(target);
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

int bookCmp(const void *a, const void *b) {
    const struct Book *bA = *((const struct Book **)a);
    const struct Book *bB = *((const struct Book **)b);

    int r = strcmp(bA->name, bB->name);

    if (r < 0) {
        return -1;
    } else if (r > 0) {
        return 1;
    } else {
        return 0;
    }
}

void filterAndPrint(struct Book **lst, int len, char *p) {
    for (int i = 0; i < len; i++) {
        if ((strstr(lst[i]->name, p) != NULL) && (lst[i]->valid)) {
            printBook(lst[i]);
        }
    }
}

void printBook(struct Book *target) {
    printf("%-50s%-20s%-30s%-10s\n", target->name, target->author,
           target->press, target->date);
}

void filterAndDelete(struct Book **lst, int len, char *p) {
    for (int i = 0; i < len; i++) {
        if ((strstr(lst[i]->name, p) != NULL) && (lst[i]->valid)) {
            lst[i]->valid = 0;
        }
    }
}

void save(struct Book **lst, int len) {
    FILE *out = fopen("ordered.txt", "w");
    if (out == NULL) {
        printf("Cannot open ordered.txt");
        exit(1);
    }

    for (int i = 0; i < len; i++) {
        if (lst[i]->valid) {
            fprintf(out, "%-50s%-20s%-30s%-10s\n", lst[i]->name, lst[i]->author,
                    lst[i]->press, lst[i]->date);
        }
    }
}