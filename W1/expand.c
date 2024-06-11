/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>
#include <string.h>
#define MAXLEN 10005

char *s_gets(char *dst, int n);
int check_range(char c);
int check_pair(char *c);

int main(void) {
    char str[MAXLEN] = {0};
    s_gets(str, MAXLEN);

    int len = (int)strlen(str);
    int sign = 0;
    for (int i = 0; i < len; i++) {
        if (check_range(str[i])) {
            if (str[i + 1] != '\0' && str[i + 1] == '-') {
                if (check_pair(str + i) == 1) {
                    char j;
                    if (sign == 1) {
                        j = str[i] + 1;
                    } else {
                        j = str[i];
                    }
                    for (; j <= str[i + 2]; j++) {
                        putchar(j);
                    }
                    if (str[i + 3] != '-') {
                        sign = 0;
                        i += 2;
                    } else {
                        sign = 1;
                        i += 1;
                    }
                } else {
                    putchar(str[i]);
                }
            } else {
                putchar(str[i]);
            }
        } else {
            putchar(str[i]);
        }
    }

    return 0;
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

int check_range(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9')) {
        return 1;
    }
    return 0;
}

int check_pair(char *c) {
    if (*c >= 'a' && *c <= 'z') {
        if (*(c + 2) >= 'a' && *(c + 2) <= 'z') {
            if (*(c + 2) > *c) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    if (*c >= 'A' && *c <= 'Z') {
        if (*(c + 2) >= 'A' && *(c + 2) <= 'Z') {
            if (*(c + 2) > *c) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    if (*c >= '0' && *c <= '9') {
        if (*(c + 2) >= '0' && *(c + 2) <= '9') {
            if (*(c + 2) > *c) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}