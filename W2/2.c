/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUTLEN 1085

char *s_gets(char *dst, int n);
char *replace(char *begin, char *end, char *content);

int main(void) {
    char from[INPUTLEN] = {0};
    char to[INPUTLEN] = {0};

    s_gets(from, INPUTLEN);
    s_gets(to, INPUTLEN);

    FILE *IN;
    if ((IN = fopen("filein.txt", "r")) == NULL)

    {

        printf("Can't open filein.txt!");

        return 1;
    }

    FILE *OUT;
    if ((OUT = fopen("fileout.txt", "w")) == NULL)

    {

        printf("Can't open fileout.txt!");

        return 1;
    }

    char buffer[INPUTLEN] = {0};
    char buffer_nocase[INPUTLEN] = {0};
    char from_nocase[INPUTLEN] = {0};
    int from_len = (int)strlen(from);
    while (fgets(buffer, INPUTLEN, IN) != NULL) {
        puts(buffer);
        int buffer_len = (int)strlen(buffer);
        for (int i = 0; i < buffer_len; i++) {
            buffer_nocase[i] = (char)tolower(buffer[i]);
        }

        for (int i = 0; i < from_len; i++) {
            from_nocase[i] = (char)tolower(from[i]);
        }
        long offset = 0;
        while (1) {
            char *found = strstr(buffer_nocase + offset, from_nocase);
            if (found == NULL) {
                break;
            } else {
                offset = found - buffer_nocase;
            }
            replace(buffer + offset, buffer + offset + from_len, to);
            char *after = replace(buffer_nocase + offset,
                                  buffer_nocase + offset + from_len, to);
            offset = after - buffer_nocase;
        }

        fputs(buffer, OUT);
    }

    fclose(IN);
    fclose(OUT);
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

char *replace(char *begin, char *end, char *content) {
    int end_len = (int)strlen(end);
    char *end_temp = (char *)malloc((unsigned)(end_len + 1) * sizeof(char));
    strcpy(end_temp, end);
    for (; *content != '\0'; begin++, content++) {
        *begin = *content;
    }
    char *ret = begin;
    for (int i = 0; i < end_len; i++) {
        *begin = end_temp[i];
        begin++;
    }
    *begin = '\0';
    free(end_temp);
    return ret;
}