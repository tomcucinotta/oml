#ifndef __OML_THREAD_H__
#define __OML_THREAD_H__

#include "oml_debug.h"
#include "oml_func.h"

#include <pthread.h>

/**
 * @file
 * 
 * @brief 
 */

#define oml_thread()				\
  rv name##__(params);                          \
  static rv name(params) {                      \
    void *__func_args, *__func_rv;              \
    pthread_mutex_lock(&m);			\
    oml_log_debug("Acquired lock: " #m);	\
    __func_args = __builtin_apply_args();       \
    __func_rv = __builtin_apply((__oml_void_func) name##__, __func_args, OML_FUNC_MAX_ARGS_SIZE); \
    oml_log_debug("Releasing lock: " #m);	\
    pthread_mutex_unlock(&m);			\
    __builtin_return(__func_rv);                \
  }                                             \
  rv name##__(params) 


#endif /* __OML_THREAD_H__ */
