#include <stdio.h>
#include <stdlib.h>
// 修改Number的具体类型时需修改getMatrix、printMatrix
typedef long long Number;

Number ** allocMatrix(int m, int n);
void freeMatrix(Number ** target, int m, int n);
void getMatrix(Number ** target, int m, int n);
void clearMatrix(Number ** target, int m, int n);
void printMatrix(Number ** target, int m, int n);
void addMatrix(Number ** result, Number ** a, Number ** b, int m, int n);
void subtractMatrix(Number ** result, Number ** a, Number ** b, int m, int n);
void copy(Number ** target, Number ** source, int m, int n);
int main(void)
{
    int n = 0;
    scanf("%d", &n);
    Number ** a = allocMatrix(n, n);
    Number ** input = allocMatrix(n, n);
    Number ** temp = allocMatrix(n, n);
    getMatrix(a, n, n);
    while (1)
    {
        char op = 0;
        scanf("%c", &op);
        switch (op)
        {
            case '#':
                printMatrix(a, n, n);
                freeMatrix(a, n, n);
                freeMatrix(input, n, n);
                freeMatrix(temp, n, n);
                return 0;
            case '+':
                getMatrix(input, n, n);
                addMatrix(temp, a, input, n, n);
                copy(a, temp, n, n);
                break;
            case '-':
                getMatrix(input, n, n);
                subtractMatrix(temp, a, input, n, n);
                copy(a, temp, n, n);
                break;
        }
    }
    
}

Number ** allocMatrix(int m, int n)
{
    Number ** p = (Number **) malloc(m * sizeof(Number *));
    for (int i = 0; i < m; i ++)
    {
        p[i] = (Number *) malloc(n * sizeof(Number));
    }
    return p;
}

void freeMatrix(Number ** target, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        free(target[i]);
        target[i] = NULL;
    }
    free(target);
}

void clearMatrix(Number ** target, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        for (int j = 0; j < n; j ++)
        {
            target[i][j] = 0;
        }
    }
}

void getMatrix(Number ** target, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        for (int j = 0; j < n; j ++)
        {
            scanf("%lld", &target[i][j]);
        }
    }
}

void printMatrix(Number ** target, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        int j;
        for (j = 0; j < (n - 1); j ++)
        {
            printf("%5lld", target[i][j]);
        }
        printf("%5lld\n", target[i][j]);
    }
}

void addMatrix(Number ** result, Number ** a, Number ** b, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        for (int j = 0; j < n; j ++)
        {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
}

void subtractMatrix(Number ** result, Number ** a, Number ** b, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        for (int j = 0; j < n; j ++)
        {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
}

void copy(Number ** target, Number ** source, int m, int n)
{
    for (int i = 0; i < m; i ++)
    {
        for (int j = 0; j < m; j ++)
        {
            target[i][j] = source[i][j];
        }
    }
}