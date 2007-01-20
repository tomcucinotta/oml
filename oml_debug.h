#ifndef __OML_DEBUG_H__
#define __OML_DEBUG_H__

/** @file
 *
 * @brief Space-Independent debugging functions.
 *
 * Most macros in this file may be called for logging or debugging
 * both from kernel-space and from user-space. They automatically
 * log messages with printk or fprintf(stderr, ...).
 *
 */

/************************************************************
 *                      ERROR CODES                         *
 ************************************************************/

/** @name QoS Library success and error return values
 * 
 * @{
 */

/** The QoS Library return value, either OML_OK or one of the OML_E_error codes.
 * 
 *  It may be converted to/from an integer through the oml_rv_int() and oml_int_rv() macros.
 *  
 *  @see oml_rv_int oml_int_rv
 * 
 *  @note Defined as a pointer in order to generate compile-time warnings in case
 *  of assignment with unallowed values (e.g. integers).
 */
typedef struct oml_rv { } *oml_rv;

#define oml_rv_int(rv) ((int)(rv))
#define oml_int_rv(err) ((oml_rv) err)

  /** Operation successfully completed          */
#define OML_OK                  oml_int_rv(0)
  /** Unspecified generic error                 */
#define OML_E_GENERIC           oml_int_rv(-16)
  /** Memory insufficient for the operation     */
#define OML_E_NO_MEMORY oml_int_rv(-17)
  /** Invalid input parameter(s)                */
#define OML_E_INVALID_PARAM     oml_int_rv(-18)
  /** Operation has not been authorized according
   * to the configured security policy          */
#define OML_E_UNAUTHORIZED      oml_int_rv(-19)
  /** Operation is not (yet) implemented        */
#define OML_E_UNIMPLEMENTED     oml_int_rv(-20)
  /** Dynamic loadable component not found, e.g. kernel module  */
#define OML_E_MISSING_COMPONENT oml_int_rv(-21)
  /** Internal state inconsistent with operation                */
#define OML_E_INCONSISTENT_STATE oml_int_rv(-22)
  /** Request rejected due to a temporary system overload       */
#define OML_E_SYSTEM_OVERLOAD   oml_int_rv(-23)
  /** Internal error: report to authors, please                 */
#define OML_E_INTERNAL_ERROR    oml_int_rv(-24)
  /** Operation could not be completed because the object was "full" */
#define OML_E_FULL oml_int_rv(-25)
  /** Operation could not be completed because the object was "empty" */
#define OML_E_EMPTY oml_int_rv(-26)
  /** Operation could not be completed because an element was not found */
#define OML_E_NOT_FOUND oml_int_rv(-27)

/** @} */

#ifdef __KERNEL__
#  include <linux/kernel.h>
#  include <linux/module.h>
#  include "kal_timer.h"
#  define OML_FFLUSH
#  ifdef __LOGGER__
#    include "logger.h"
#    define debug_print log_printf
#    define DBG_DEV 0,
#  else
#    define debug_print printk
#    define DBG_DEV
#  endif
#define OML_DEBUG_WITH_TIME
#  ifdef OML_DEBUG_WITH_TIME
     extern kal_time_t sched_time;
#    define DBG_TIME_FORMAT KAL_TIME_FMT
#    define DBG_TIME KAL_TIME_FMT_ARG(kal_time_now()),
#  else
#    define DBG_TIME_FORMAT
#    define DBG_TIME
#  endif
#else
#  include <stdio.h>
#  include <unistd.h>
#  include <stdlib.h>
#  define debug_print fprintf
#  define DBG_DEV stderr,
#  define OML_FFLUSH fflush(stderr)
#  define DBG_TIME_FORMAT
#  define DBG_TIME
#endif

#ifndef OML_GLOBAL_LEVEL
#  define OML_GLOBAL_LEVEL OML_LEVEL_DEBUG
#endif

extern int oml_log_msg_id;

/** Convert a OML_E_* error code into a string.
 *
 * Returns a pointer to a statically allocated string with a human
 * readable description of the error coded into the errno parameter.
 */
char *oml_strerror(oml_rv errno);

/** For internal use only       */
#define oml_log_str(level,str,msg,args...)                      \
do {                                                            \
  int __level = (level);                                        \
  if (__level <= OML_DEBUG_LEVEL && __level <= OML_GLOBAL_LEVEL) {  \
    debug_print(DBG_DEV DBG_TIME_FORMAT "[%6d][%15s] " str msg "\n", DBG_TIME oml_log_msg_id, __func__ , ##args); \
    OML_FFLUSH;                                                 \
    oml_log_msg_id++;                                                   \
  }                                                             \
} while (0)

/** Log a message without function name and extra information */
#define oml_log_simple(level,msg,args...) \
do {            \
  if ((level) <= OML_DEBUG_LEVEL) {   \
    debug_print(DBG_DEV msg "\n", ##args);  \
    OML_FFLUSH;         \
  }           \
} while (0)

/** No debug: no log messages are generated                     */
#define OML_LEVEL_NODEBUG 0
/** Error messages: always reported if OML_DEBUG_LEVEL >= 1     */
#define OML_LEVEL_ERROR 1
/** Warning messages: reported only if OML_DEBUG_LEVEL >= 2     */
#define OML_LEVEL_WARN 2
/** Information messages: reported only if OML_DEBUG_LEVEL >= 3 */
#define OML_LEVEL_INFO 3
/** Debug messages: reported only if OML_DEBUG_LEVEL >= 4       */
#define OML_LEVEL_DEBUG 4
/** Verbose messages: reported only if OML_DEBUG_LEVEL >= 8     */
#define OML_LEVEL_VERB 5

#ifndef OML_DEBUG_LEVEL
/** OML_DEBUG_LEVEL sets the verbosity level of debug messages
 * (see OML_DEBUG_xx constants for details).
 *
 * OML_DEBUG_LEVEL is set by default to 1 (warnings and errors)
 */
#  define OML_DEBUG_LEVEL OML_LEVEL_WARN
#endif /* OML_DEBUG_LEVEL */

/** Log a message with a specified level        */
#define oml_log(level,msg,args...) oml_log_str(level,"",msg,##args)

/** Log an error message        */
#define oml_log_err(msg,args...) oml_log_str(OML_LEVEL_ERROR,   "<ERR> ",msg,##args)

/** Log a warning message       */
#define oml_log_warn(msg,args...) oml_log_str(OML_LEVEL_WARN,  "<WRN> ",msg,##args)

/** Log an informative message  */
#define oml_log_info(msg,args...) oml_log_str(OML_LEVEL_INFO,  "<INF> ",msg,##args)

/** Log a debugging message     */
#define oml_log_debug(msg,args...) oml_log_str(OML_LEVEL_DEBUG, "<DBG> ",msg,##args)

/** Log a verbose debugging message     */
#define oml_log_verb(msg,args...) oml_log_str(OML_LEVEL_VERB, "<VRB> ",msg,##args)

/** Log a variable. 
 *  Example: to print the long int variable "my_long_int" as a debug message  -> oml_log_var(OML_LEVEL_DEBUG, my_long_int, "%ld") */
#define oml_log_var(level, __var_name, __var_type) oml_log(level, #__var_name ": " __var_type, __var_name)

/** Log a critical error: always reported.      */
#define oml_log_crit(msg,args...)                       \
do {                                                    \
  debug_print(DBG_DEV "[%15s] <CRIT> " msg "\n", __func__ , ##args); \
  OML_FFLUSH;                                           \
} while (0)

/** Evaluate condition and log a warning message if it is not satisfied.
 *
 * Condition is always evaluated, independently of debug settings.
 */
#define oml_chk(cond) do {                                                      \
  if (!(cond)) {                                                                \
    oml_log_warn("Condition '" #cond "' not satisfied at line %d of file %s.",\
             __LINE__, __FILE__);                                               \
  }                                                                             \
} while (0)

/**
 * Evaluate condition, and, if unsatisfied, then log a warning message and return
 * from current function.
 *
 * Condition is always evaluated, independently of debug settings.
 */
#define oml_chk_rv(cond,rv) do {                                                        \
  if (!(cond)) {                                                                \
    oml_log_warn("Condition '" #cond "' not satisfied at line %d of file %s.",\
             __LINE__, __FILE__);                                               \
    return rv;                                                                  \
  }                                                                             \
} while (0)

/** Evaluate expr and log a warning message with stringified
 *  error if it does not evaluate to OML_OK.
 *
 * Expression is always evaluated, independently of debug settings.
 */
#define oml_chk_ok(expr) do {                                           \
  oml_rv __rv = (expr);                                                 \
  if (__rv != OML_OK) {                                                 \
    oml_log_warn("Check failed: '" #expr " = %s' at line %d of file %s.",\
             oml_strerror(__rv), __LINE__, __FILE__);                   \
  }                                                                     \
} while (0)

/** Evaluate expr once and, if it does not evaluate to OML_OK, then log a
 * warning message with the stringified error and return it to the caller.
 *
 * Expression is always evaluated, independently of debug settings.
 */
#define oml_chk_ok_ret(expr) do {           \
  oml_rv __rv = (expr);              \
  if (__rv != OML_OK) {             \
    oml_log_warn("Check failed: '" #expr " = %s' at line %d of file %s.",\
       oml_strerror(__rv), __LINE__, __FILE__);     \
    return __rv; \
  }                 \
} while (0)

/** Evaluate cond and log a custom message with args if it is not satisfied.
 *
 * Allows to specify a custom printf-like format message with arguments.
 * Condition is always evaluated, independently of debug settings.
 */
#define oml_chk_msg(cond,msg,args...) do {                      \
  if (!(cond)) {                                                \
    oml_log_warn("Check failed: '" #cond                        \
      "' at line %d of file %s.", __LINE__, __FILE__);          \
  }                                                             \
} while (0)

/** Evaluate cond and, if it is not satisfied, log a warning and go to label lab.
 *
 * Allows to specify a custom printf-like format message with arguments.
 * Condition is always evaluated, independently of debug settings.
 */
#define oml_chk_go(cond,lab,msg,args...) do {                   \
  if (!(cond)) {                                                \
    oml_log_str(OML_LEVEL_WARN,   "<WRN> ",msg,##args);         \
    goto lab;                                                   \
  }                                                             \
} while (0)

/** Check if debug is enabled for (at least) the specified level        */
#define oml_debug_enabled_for(lev) (OML_DEBUG_LEVEL >= (lev))

/** Evaluate cond and log a custom message with args if it is not satisfied.
 * 
 * Differently from oml_chk_msg, this macro evaluates condition only
 * if the debug level is enabled for OML_LEVEL_DEBUG
 */
#define oml_chk_if(cond,msg,args...) do {                       \
  if (oml_debug_enabled_for(OML_LEVEL_DEBUG)) {                 \
    if (!(cond))                                                \
      oml_log_str(OML_LEVEL_WARN,   "<WRN> ",msg,##args)        \
  }                                                             \
} while (0)

#ifdef __KERNEL__

#else   /* __KERNEL__ */

/** Evaluate cond and cause exit(-1) if it is not satisfied.
 *
 * Only available in user-space.
 * 
 * This check happens always, independently of debug settings.
 */
#define oml_chk_exit(cond) do {                                         \
  int ok = (cond);                                                      \
  if (! ok) {                                                           \
    oml_log_err("ASSERTION FAILED: '" #cond "' at line %d of file %s.",\
             __LINE__, __FILE__);                                       \
    exit(-1);                                                           \
  }                                                                     \
} while (0)

/** Evaluate expr and, if it does not evaluate to OML_OK, prints the
 *  stringified error and causes exit(-1).
 *
 * Only available in user-space */
#define oml_chk_ok_exit(expr) do {                                      \
  oml_rv __rv = (expr);                                                 \
  if (__rv != OML_OK) {                                                 \
    oml_log_crit("ASSERTION FAILED: '" #expr " = %s' at line %d of file %s.",\
             oml_strerror(__rv), __LINE__, __FILE__);                   \
    exit(-1);                                                           \
  }                                                                     \
} while (0)

/** Evaluate cond and cause exit(-1) if it is not satisfied.
 *
 * Only available in user-space.
 * 
 * This check is only compiled if debug level is at least WARN.
 */
#define oml_assert(cond) do {                                           \
  if (oml_debug_enabled_for(OML_LEVEL_WARN)) {                 \
          int ok = (cond);                                                      \
          if (! ok) {                                                           \
            oml_log_err("ASSERTION FAILED: '" #cond "' at line %d of file %s.",\
                     __LINE__, __FILE__);                                       \
            exit(-1);                                                           \
          }                                                                     \
  } \
} while (0)

/** Evaluate cond and cause exit(-1) if it is not satisfied.
 *
 * Only available in user-space.
 * 
 * This check is only compiled if debug level is at least WARN.
 */
#define oml_assert_ok(expr) do {                                           \
  if (oml_debug_enabled_for(OML_LEVEL_WARN)) {                 \
          oml_rv __rv = (expr);                                                 \
          if (__rv != OML_OK) {                                                 \
            oml_log_crit("ASSERTION FAILED: '" #expr " = %s' at line %d of file %s.",\
                     oml_strerror(__rv), __LINE__, __FILE__);                   \
            exit(-1);                                                           \
          }                                                                     \
  } \
} while (0)

#endif  /* __KERNEL__ */

#endif /* __OML_DEBUG_H__ */
