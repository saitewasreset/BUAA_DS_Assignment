#include <stdio.h>

int main(void) {
    int count = 0;
    int n = 20;
    int i = 1;
    int j = 0;
    while (i + j <= n) {
        if (i > j) {
            j++;
            count++;
        } else
            i++;
    }
    printf("%d\n", count);
    return 0;
}