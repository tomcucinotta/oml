#ifndef OML_FUNC_H_
#define OML_FUNC_H_

#include "oml_debug.h"

#ifndef OML_FUNC_MAX_ARGS_SIZE
/** The default maximum size (in bytes) of functions defined through OML wrappers.
 **
 ** Each wrapped function will require, at runtime, at least OML_FUNC_MAX_ARGS_SIZE
 ** bytes in the stack for the call (if debugging is enabled). Therefore, override
 ** this value to greater values only when actually needed.
 **
 ** @note
 ** This value may be overridden by defining this macro to something
 ** different than 64 before inclusion of oml_func.h.
 **/
#  define OML_FUNC_MAX_ARGS_SIZE 64
#endif

typedef void (* __oml_void_func)(void);

/** @note
 ** This works only with functions that require less than
 ** OML_FUNC_MAX_ARGS_SIZE bytes in the stack.
 ** for passing arguments.
 **/
#define oml_func_define(rv, name, params...)    \
  rv name##__(params);                          \
  static rv name(params) {                      \
    void *__func_args, *__func_rv;              \
    oml_log_debug("Entering function: " #name); \
    __func_args = __builtin_apply_args();       \
    __func_rv = __builtin_apply((__oml_void_func) name##__, __func_args, OML_FUNC_MAX_ARGS_SIZE); \
    oml_log_debug("Leaving function: " #name);  \
    __builtin_return(__func_rv);                \
  }                                             \
  rv name##__(params) 

#define oml_traced(name)			\
  rv name##__(void);                            \
  static rv name(void) {                        \
    void *__func_args, *__func_rv;              \
    oml_log_debug("Entering block: " #name);    \
    __func_args = __builtin_apply_args();       \
    __func_rv = __builtin_apply((__oml_void_func) name##__, __func_args, 0); \
    oml_log_debug("Leaving block: " #name);     \
    __builtin_return(__func_rv);                \
  }                                             \
  rv name##__(params) 

#endif /*OML_FUNC_H_*/
