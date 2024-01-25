#ifndef __OML_SYNC_H__
#define __OML_SYNC_H__

#include "oml_debug.h"
#include "oml_func.h"

#include <pthread.h>

/**
 * @file
 * Collections of wrappers for multi-thread support
 * @brief 
 */

#define oml_mutex pthread_mutex_t

#define oml_sync(p_mtx, rv, function_name, params...)    \
  rv function_name##__(params);                          \
  static rv function_name(params) {                      \
    void *__func_args, *__func_rv;              \
    pthread_mutex_lock(p_mtx);			\
    oml_log_debug("Acquired lock : *" #p_mtx);	\
    __func_args = __builtin_apply_args();       \
    __func_rv = __builtin_apply((__oml_void_func) function_name##__, __func_args, OML_FUNC_MAX_ARGS_SIZE); \
    oml_log_debug("Releasing lock: *" #p_mtx);	\
    pthread_mutex_unlock(p_mtx);		\
    __builtin_return(__func_rv);                \
  }                                             \
  rv function_name##__(params) 

#define oml_sync_named_region(name, p_mutex)    \
  auto void __sync## name (void);               \
  pthread_mutex_lock(p_mutex);			\
  __sync## name ();				\
  pthread_mutex_unlock(p_mutex);		\
  void __sync## name (void) 

#define oml_sync_named_region2(name, p_mutex)   \
  oml_sync_named_region(name, (p_mutex))

#define oml_sync_region(p_mutex) \
  oml_sync_named_region2(__COUNTER__, (p_mutex))

#define oml_mutex_init(p_mtx) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (pthread_mutex_init(p_mtx, NULL) != 0) { \
        __rv = OML_E_INTERNAL_ERROR; \
        perror("pthread_mutex_init: "); \
        break; \
    } \
  } while (0); \
  __rv; \
})

#define oml_mutex_destroy(p_mtx) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (pthread_mutex_destroy(p_mtx) != 0) { \
        __rv = OML_E_INTERNAL_ERROR; \
        perror("pthread_mutex_destroy: "); \
        break; \
    } \
  } while (0); \
  __rv; \
})

#define oml_mutex_lock(p_mtx) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (pthread_mutex_lock(p_mtx) != 0) { \
        __rv = OML_E_INTERNAL_ERROR; \
        perror("pthread_mutex_lock: "); \
        break; \
    } \
  } while (0); \
  __rv; \
})

#define oml_mutex_unlock(p_mtx) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (pthread_mutex_unlock(p_mtx) != 0) { \
        __rv = OML_E_INTERNAL_ERROR; \
        perror("pthread_mutex_lock: "); \
        break; \
    } \
  } while (0); \
  __rv; \
})
#endif /* __OML_SYNC_H__ */
