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

#define oml_sync_region(p_mutex)			\
  auto void __sync(void);			\
  pthread_mutex_lock(p_mutex);			\
  oml_log_debug("Acquired mutex : *" #p_mutex);	\
  __sync();					\
  oml_log_debug("Releasing mutex: *" #p_mutex);	\
  pthread_mutex_unlock(p_mutex);		\
  void __sync(void) 

#endif /* __OML_SYNC_H__ */
