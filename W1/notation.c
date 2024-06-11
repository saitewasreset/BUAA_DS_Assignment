/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>
#include <string.h>

#define MAXLEN 105

char *s_gets(char *dst, int n);

int main(void) {
    char str[MAXLEN] = {0};
    s_gets(str, MAXLEN);

    int len = (int)strlen(str);

    char *point = strchr(str, '.');
    if (point == NULL) {
        point = str + len;
    }

    int front_len = (int)(point - str);

    if ((front_len == 1) && (*(point - 1) == '0')) {
        // 0.000000000000002
        int exp = -1;
        point++;
        for (; *point == '0'; point++) {
            exp--;
        }
        int out_len = (int)strlen(point);
        if (out_len == 1) {
            printf("%se%d", point, exp);
        } else {
            putchar(point[0]);
            putchar('.');
            for (int i = 1; i < out_len; i++) {
                putchar(point[i]);
            }
            printf("e%d", exp);
        }
    } else {
        int exp = 0;
        point--;
        for (; point != str; point--) {
            exp++;
        }

        putchar(str[0]);
        putchar('.');

        for (int i = 1; i < len; i++) {
            if (str[i] != '.') {
                putchar(str[i]);
            }
        }

        printf("e%d", exp);
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
