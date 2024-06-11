#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b);
size_t selectSort(int *arr, size_t numberOfElement);
size_t bubbleSort(int *arr, size_t numberOfElement);
size_t heapSort(int *arr, size_t numberOfElement);
size_t heapAdjust(int *arr, size_t i, size_t n);

size_t merge(int *x, int *tmp, size_t left, size_t leftend, size_t rightend);
size_t mSort(int *x, int *temp, size_t left, size_t right);
size_t mergeSort(int *arr, size_t numberOfElement);

size_t _quickSort(int *arr, size_t left, size_t right);
size_t quickSort(int *arr, size_t numberOfElement);

int main(void) {
    size_t len = 0;
    int mode = 0;

    scanf("%zu%d", &len, &mode);

    int *arr = (int *)malloc(len * sizeof(int));

    for (size_t i = 0; i < len; i++) {
        scanf("%d", &arr[i]);
    }

    size_t cmpCount = 0;
    switch (mode) {
    case 1:
        cmpCount = selectSort(arr, len);
        break;
    case 2:
        cmpCount = bubbleSort(arr, len);
        break;
    case 3:
        cmpCount = heapSort(arr, len);
        break;
    case 4:
        cmpCount = mergeSort(arr, len);
        break;
    case 5:
        cmpCount = quickSort(arr, len);
        break;
    }

    for (size_t i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n%zu\n", cmpCount);
    free(arr);

    return 0;
}

size_t selectSort(int *arr, size_t numberOfElement) {
    int temp = 0;
    size_t cmpCount = 0;
    for (size_t i = 0; i < numberOfElement; i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < numberOfElement; j++) {
            cmpCount++;
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
    return cmpCount;
}

size_t bubbleSort(int *arr, size_t numberOfElement) {
    size_t cmpCount = 0;
    if (numberOfElement == 0) {
        return 0;
    }
    int temp = 0;
    for (size_t i = numberOfElement - 1; i > 0; i--) {
        bool flag = true;
        for (size_t j = 0; j < i; j++) {
            cmpCount++;
            if (arr[j] > arr[j + 1]) {
                flag = false;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        if (flag) {
            break;
        }
    }
    return cmpCount;
}

size_t heapAdjust(int *arr, size_t i, size_t n) {
    size_t j;
    int temp;
    size_t cmpCount = 0;
    temp = arr[i];
    j = 2 * i + 1;
    while (j < n) {
        if (j < n - 1 && arr[j] < arr[j + 1])
            j++;
        cmpCount++;
        if (temp >= arr[j])
            break;
        arr[(j - 1) / 2] = arr[j];
        j = 2 * j + 1;
    }
    arr[(j - 1) / 2] = temp;
    return cmpCount;
}

size_t heapSort(int *arr, size_t numberOfElement) {
    if (numberOfElement <= 1) {
        return 0;
    }
    size_t cmpCount = 0;
    int temp = 0;

    for (size_t i = 0; i < numberOfElement / 2; i++) {
        cmpCount +=
            heapAdjust(arr, numberOfElement / 2 - 1 - i, numberOfElement);
    }

    temp = arr[numberOfElement - 1];
    arr[numberOfElement - 1] = arr[0];
    arr[0] = temp;

    for (size_t i = 1; i < numberOfElement; i++) {
        cmpCount += heapAdjust(arr, 0, numberOfElement - i);
        temp = arr[numberOfElement - i - 1];
        arr[numberOfElement - i - 1] = arr[0];
        arr[0] = temp;
    }

    return cmpCount;
}

size_t merge(int *x, int *tmp, size_t left, size_t leftend, size_t rightend) {
    size_t cmpCount = 0;
    size_t i = left, j = leftend + 1, q = left;
    while (i <= leftend && j <= rightend) {
        cmpCount++;
        if (x[i] <= x[j])
            tmp[q++] = x[i++];
        else
            tmp[q++] = x[j++];
    }
    while (i <= leftend)
        tmp[q++] = x[i++];
    while (j <= rightend)
        tmp[q++] = x[j++];
    for (i = left; i <= rightend; i++)
        x[i] = tmp[i];
    return cmpCount;
}

size_t mSort(int *x, int *temp, size_t left, size_t right) {
    size_t cmpCount = 0;

    if (left < right) {
        size_t center = (left + right) / 2;
        cmpCount += mSort(x, temp, left, center);
        cmpCount += mSort(x, temp, center + 1, right);
        cmpCount += merge(x, temp, left, center, right);
    }

    return cmpCount;
}

size_t mergeSort(int *arr, size_t numberOfElement) {
    size_t cmpCount = 0;

    if (numberOfElement <= 1) {
        return 0;
    }

    int *temp = (int *)malloc(numberOfElement * sizeof(int));

    cmpCount += mSort(arr, temp, 0, numberOfElement - 1);

    free(temp);
    return cmpCount;
}

size_t quickSort(int *arr, size_t numberOfElement) {
    return _quickSort(arr, 0, numberOfElement - 1);
}

size_t _quickSort(int *arr, size_t left, size_t right) {
    size_t cmpCount = 0;
    size_t i, last;
    int temp = 0;
    if (left < right) {
        last = left;
        for (i = left + 1; i <= right; i++) {
            cmpCount++;
            if (arr[i] < arr[left]) {
                last++;
                temp = arr[last];
                arr[last] = arr[i];
                arr[i] = temp;
            }
        }
        temp = arr[left];
        arr[left] = arr[last];
        arr[last] = temp;
        cmpCount += _quickSort(arr, left, last - 1);
        cmpCount += _quickSort(arr, last + 1, right);
    }
    return cmpCount;
}
