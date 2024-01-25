#include <pthread.h>
#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_sync.h"
#include <errno.h>

int main(int argc, char *argv[]) {
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

  int cnt = 0;

  oml_sync_region(&m) {
    int err;
    if ((err = pthread_mutex_trylock(&m)) != 0)
      oml_log_debug("pthread_mutex_trylock() failed as expected: %d", err);
    oml_chk(err == EBUSY);
    cnt += 1;
    printf("1st: args=%d, cnt=%d\n", argc, cnt);
    return;
  }

  oml_sync_region(&m) {
    int err;
    if ((err = pthread_mutex_trylock(&m)) != 0)
      oml_log_debug("pthread_mutex_trylock() failed as expected: %d", err);
    oml_chk(err == EBUSY);
    cnt += 10;
    printf("2nd: args=%d, cnt=%d\n", argc, cnt);
    return;
  }

  printf("cnt=%d\n", cnt);

  oml_chk(cnt == 11);

  return 0;
}
