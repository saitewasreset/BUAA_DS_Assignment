/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>
#include <string.h>

#define MAXLEN 85

int bigIntCmp(const char *a, const char *b);
char *s_gets(char *dst, int n);
void read(char *s);
int min(int a, int b);
void unsigned_subtract(char *a, char *b);
void print(char *unsigned_input, int sign);

int main(void) {
    /*
    char temp[MAXLEN] = {0};
    read(temp);
    puts(temp);*/
    /*
    char a[MAXLEN] = "12900";
    char b[MAXLEN] = "789";
    unsigned_subtract(a, b);
    puts(a);
    */
    char a[MAXLEN] = {0};
    char b[MAXLEN] = {0};
    read(a);
    read(b);
    if (bigIntCmp(a, b) >= 0) {
        unsigned_subtract(a, b);
        print(a, 0);
    } else {
        unsigned_subtract(b, a);
        print(b, 1);
    }
    return 0;
}

int bigIntCmp(const char *a, const char *b) {
    if (strlen(a) < strlen(b)) {
        return -1;
    } else if (strlen(a) > strlen(b)) {
        return 1;
    } else {
        return strcmp(a, b);
    }
}

void read(char *s) {
    char temp[MAXLEN] = {0};
    s_gets(temp, MAXLEN);
    char *input = temp;

    while ((*input == '0') && (*input != '\0')) {
        input++;
    }

    if (*input == '\0') {
        strcpy(s, "0");
    } else {
        strcpy(s, input);
    }
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

// require: a >= b
void unsigned_subtract(char *a, char *b) {
    int aLen = (int)strlen(a);
    int bLen = (int)strlen(b);
    int limit = min(aLen, bLen);

    for (int i = 0; i < limit; i++) {
        int aIndex = aLen - i - 1;
        int bIndex = bLen - i - 1;

        if (a[aIndex] >= b[bIndex]) {
            a[aIndex] = (char)(a[aIndex] - b[bIndex] + '0');
        } else {
            a[aIndex] = (char)(10 + a[aIndex] - b[bIndex] + '0');
            a[aIndex - 1]--;
        }
    }

    for (int i = aLen - limit - 1; i >= 1; i--) {
        if (a[i] < '0') {
            a[i] += 10;
            a[i - 1]--;
        }
    }
}

inline int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

void print(char *unsigned_input, int sign) {
    while ((*unsigned_input == '0') && (*unsigned_input != '\0')) {
        unsigned_input++;
    }

    if (*unsigned_input == '\0') {
        printf("0\n");
    } else {
        if (sign == 1) {
            putchar('-');
        }
        puts(unsigned_input);
    }
}