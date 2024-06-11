#include <stdio.h>

int main(void) {
    char buffer[2048] = {0};
    fread(buffer, sizeof(char), 2048, stdin);
    fwrite(buffer, sizeof(char), 2048, stdout);
    return 0;
}