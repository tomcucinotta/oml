#include <pthread.h>
#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"

#include <pthread.h>

#define oml_sync(p_mutex)			\
  auto void __sync(void);			\
  pthread_mutex_lock(p_mutex);			\
  oml_log_debug("Acquired mutex : *" #p_mutex);	\
  __sync();					\
  oml_log_debug("Releasing mutex: *" #p_mutex);	\
  pthread_mutex_unlock(p_mutex);		\
  void __sync(void) 

int main(int argc, char *argv[]) {
  pthread_mutex_t m;

  oml_sync(&m) {
    printf("args=%d\n", argc);
    return;
  }

  return 0;
}
