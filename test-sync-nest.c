#include <pthread.h>
#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_sync.h"
#include <errno.h>

int main(int argc, char *argv[]) {
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

  oml_sync_region(&m) {
    int err;
    if ((err = pthread_mutex_trylock(&m)) != 0)
      oml_log_debug("pthread_mutex_trylock() failed as expected: %d", err);
    oml_chk(err == EBUSY);
    printf("args=%d\n", argc);
    return;
  }

  return 0;
}
