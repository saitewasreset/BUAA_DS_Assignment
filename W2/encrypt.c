/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INPUTLEN 80

char *s_gets(char *dst, int n);

int used[26] = {0};

char alpha_map[27] = {0};

int main(void) {
    char input[INPUTLEN] = {0};
    s_gets(input, INPUTLEN);
    int input_len = (int)strlen(input);

    for (int i = 0; i < input_len; i++) {
        if (isalpha(input[i])) {
            if (used[input[i] - 'a'] == 1) {
                input[i] = '0';
            } else {
                used[input[i] - 'a'] = 1;
            }
        }
    }

    int processed_len = 0;
    for (int i = 0; i < input_len; i++) {
        if (isalpha(input[i])) {
            alpha_map[processed_len] = input[i];
            processed_len++;
        }
    }

    for (int i = 25; i >= 0; i--) {
        if (used[i] == 0) {
            alpha_map[processed_len] = (char)('a' + i);
            processed_len++;
        }
    }

    FILE *in;
    FILE *out;
    if ((in = fopen("encrypt.txt", "r")) == NULL) {
        printf("Cannot open encrypt.txt\n");
        return 1;
    }
    if ((out = fopen("output.txt", "w")) == NULL) {
        printf("Cannot open output.txt\n");
        return 1;
    }

    int ch;
    while ((ch = fgetc(in)) != EOF) {
        if (islower(ch)) {
            fputc(alpha_map[ch - 'a'], out);
        } else {
            fputc(ch, out);
        }
    }

    fclose(in);
    fclose(out);
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