#include <stdio.h>

void f() {
  printf("%p\n", f);
  printf("%p\n", __builtin_frame_address(0));
}

int main() {
  f();

  return 0;
}
