#include <stdio.h>
#include <string.h>
int main() {
  char a[7] = "abcdef", b[4] = "ABC";
  strcpy(a, b);
  printf("%c", a[1]);
}