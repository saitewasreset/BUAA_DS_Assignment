#include <stdio.h>

int fact(int n);

int fact(int n) {
  if (n <= 1) {
    return 1;
  } else {
    return n * fact(n - 1);
  }
}

int main(void) {
  int n = 0;
  scanf("%d", &n);
  printf("%d\n", fact(n));
  return 0;
}