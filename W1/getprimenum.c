#include <stdio.h>

int isPrime(long long a);
long long iPow(long long base, long long exp);
long long reverse(long long a);
int f(long long a);
int main(void)
{
    long long input = 0;
    scanf("%lld", &input);
    if (f(input) == 1)
    {
        printf("yes");
    }
    else
    {
        printf("no");
    }
    return 0;
}

int f(long long a)
{
    long long reversed = reverse(a);
    return (isPrime(a) && isPrime(reversed));
}

int isPrime(long long a)
{
    if (a == 2)
    {
        return 1;
    }
    if (a % 2 == 0)
    {
        return 0;
    }
    for (long long i = 3; i * i < a; i += 2) 
    {
        if (a % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

long long reverse(long long a)
{
    long long digits[20] = {0};
    int len = 0;
    while (a > 0)
    {
        digits[len] = a % 10;
        a /= 10;
        len ++;
    }
    long long result = 0;
    for (long long i = 0; i < len; i ++)
    {
        result += digits[i] * iPow(10, len - i - 1);
    }
    return result;
}

long long iPow(long long base, long long exp)
{
    long long result = 1;
    for (long long i = 0; i < exp; i ++)
    {
        result *= base;
    }
    return result;
}