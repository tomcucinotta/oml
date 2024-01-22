#include <pthread.h>
#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_sync.h"

int main(int argc, char *argv[]) {
  pthread_mutex_t m;

  oml_sync_region(&m) {
    printf("args=%d\n", argc);
    return;
  }

  return 0;
}
