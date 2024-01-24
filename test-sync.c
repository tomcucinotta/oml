#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_sync.h"
#include <stdio.h>
#include <errno.h>

oml_mutex m = PTHREAD_MUTEX_INITIALIZER;

oml_sync(&m, int, f, int a, int b) {
  int err;
  if ((err = pthread_mutex_trylock(&m)) != 0)
    oml_log_debug("pthread_mutex_trylock() failed as expected: %d", err);
  oml_chk(err == EBUSY);
  oml_log_debug("Computing %d + %d", a, b);
  if (a == 0 && b == 0)
    return 0;
  return a + b;
}

int main() {
  printf("f(3, 5)=%d\n", f(3, 5));
  printf("f(0, 0)=%d\n", f(0, 0));

  return 0;
}
