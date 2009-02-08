#ifndef __OML_SYNC_H__
#define __OML_SYNC_H__

#include "oml_debug.h"
#include "oml_func.h"

#include <pthread.h>

/**
 * @file
 * 
 * @brief 
 */

#define oml_mutex pthread_mutex_t

#define oml_sync(p_mtx, rv, name, params...)    \
  rv name##__(params);                          \
  static rv name(params) {                      \
    void *__func_args, *__func_rv;              \
    pthread_mutex_lock(p_mtx);			\
    oml_log_debug("Acquired lock : *" #p_mtx);	\
    __func_args = __builtin_apply_args();       \
    __func_rv = __builtin_apply((__oml_void_func) name##__, __func_args, OML_FUNC_MAX_ARGS_SIZE); \
    oml_log_debug("Releasing lock: *" #p_mtx);	\
    pthread_mutex_unlock(p_mtx);		\
    __builtin_return(__func_rv);                \
  }                                             \
  rv name##__(params) 


#endif /* __OML_SYNC_H__ */
