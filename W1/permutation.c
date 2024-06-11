/*本次作业由本人独立实现完成，如有抄袭行为，愿承担由此带来的一切不良后果*/
#include <stdio.h>
#include <stdlib.h>

void permutation(int *arr, int len);
void _permutation(int *arr, int *result, int len, int used, int a);

int main(void) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = 0;
    scanf("%d", &n);
    permutation(arr, n);
    return 0;
}

void permutation(int *arr, int len) {
    int *temp = (int *)malloc((unsigned)len * sizeof(int));
    _permutation(arr, temp, len, 0, 0);
    free(temp);
}

void _permutation(int *arr, int *result, int len, int used, int a) {
    int _used = used;
    if (len == a) {
        for (int i = 0; i < a; i++) {
            printf("%d ", result[i]);
        }
        putchar('\n');
    }
    for (int i = 0; i < len; i++) {
        if (((_used >> arr[i]) & 1) == 0) {
            result[a] = arr[i];
            used = _used | (1 << arr[i]);
            _permutation(arr, result, len, used, a + 1);
        }
    }
}