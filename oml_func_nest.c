#include <stdio.h>

#define sync(lock)   \
  auto void __sync(void);  \
  printf("Lock on %d\n", lock);  \
  __sync();          \
  printf("Unlock on %d\n", lock); \
  void __sync(void) 

int main(int argc, char *argv[]) {
  int l = 3;

  sync(l) {
    printf("args=%d\n", argc);
  }
}
