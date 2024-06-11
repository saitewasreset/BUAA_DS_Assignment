#include "ring.h"
#include <stdio.h>
#include <string.h>
#define INPUTLEN 35
#define COUNTLEN 128
#define MAPLEN 128
#define RINGSIZE 95

char *s_gets(char *dst, int n);
void generateRingString(char *ringStr);
void encrypt(FILE *in, FILE *out, char *map);
int main(void) {

    char ringStr[RINGSIZE + 1] = {0};
    generateRingString(ringStr);
    struct Ring *ring = createRing(ringStr, RINGSIZE);

    char map[MAPLEN] = {0};

    for (int i = 0; i < MAPLEN; i++) {
        map[i] = (char)i;
    }

    generateKeyMap(ring, map);

    FILE *in = fopen("in.txt", "r");
    FILE *out = fopen("in_crpyt.txt", "w");

    encrypt(in, out, map);

    fclose(in);
    fclose(out);
    destroyRing(ring);

    return 0;
}

void generateRingString(char *ringStr) {
    char input[INPUTLEN] = {0};
    s_gets(input, INPUTLEN);
    int len = (int)strlen(input);

    // ASCII: 32-126
    int count[COUNTLEN] = {0};
    int ringCurrent = 0;
    for (int i = 0; i < len; i++) {
        if (count[(unsigned)input[i]] == 0) {
            ringStr[ringCurrent] = input[i];
            ringCurrent++;
        }
        count[(unsigned)input[i]] = 1;
    }
    // ASCII: 32-126
    for (int i = 32; i <= 126; i++) {
        if (count[i] == 0) {
            ringStr[ringCurrent] = (char)i;
            ringCurrent++;
        }
    }
    ringStr[ringCurrent] = '\0';
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

void encrypt(FILE *in, FILE *out, char *map) {
    int ch = 0;
    while ((ch = fgetc(in)) != EOF) {
        fputc(map[ch], out);
    }
}