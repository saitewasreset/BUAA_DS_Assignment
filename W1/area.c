#include <stdio.h>

int max(int a, int b);
int min(int a, int b);
int intersect(int ax1, int ax2, int bx1, int bx2);

int main(void)
{
    int ax1, ay1, ax2, ay2;
    ax1 = ay1 = ax2 = ay2 = 0;
    int bx1, by1, bx2, by2;
    bx1 = by1 = bx2 = by2 = 0;
    scanf("%d%d%d%d%d%d%d%d", &ax1, &ay1, &ax2, &ay2, &bx1, &by1, &bx2, &by2);
    int x = intersect(ax1, ax2, bx1, bx2);
    int y = intersect(ay1, ay2, by1, by2);
    printf("%d", x * y);
    return 0;
}

inline int max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

inline int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    else 
    {
        return a;
    }
}

int intersect(int ax1, int ax2, int bx1, int bx2)
{
    int max_ax = max(ax1, ax2);
    int min_ax = min(ax1, ax2);
    int max_bx = max(bx1, bx2);
    int min_bx = min(bx1, bx2);
    int temp = min(max_ax, max_bx) - max(min_ax, min_bx);
    if (temp < 0)
    {
        return 0;
    }
    else
    {
        return temp;
    }
}