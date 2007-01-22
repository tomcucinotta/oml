#include <stdio.h>


int f(int a, int b) {
  printf("header\n");
  int _f(int a, int b) {
    return a + b;
  }
  int _rv = _f(a, b);
  printf("footer\n");
  return _rv;
}

int main() {
  int z = f(3, 5);
}
