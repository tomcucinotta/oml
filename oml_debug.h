#ifndef __OML_DEBUG_H__
#define __OML_DEBUG_H__

#include <stdio.h>

/** No debug: no log messages are generated			*/
#define OML_LEVEL_NODEBUG 0
/** Error messages: always reported if OML_DEBUG_LEVEL >= 1	*/
#define OML_LEVEL_ERROR 1
/** Warning messages: reported only if OML_DEBUG_LEVEL >= 2 	*/
#define OML_LEVEL_WARN 2
/** Information messages: reported only if OML_DEBUG_LEVEL >= 3	*/
#define OML_LEVEL_INFO 3
/** Debug messages: reported only if OML_DEBUG_LEVEL >= 4 	*/
#define OML_LEVEL_DEBUG 4
/** Verbose messages: reported only if OML_DEBUG_LEVEL >= 8 	*/
#define OML_LEVEL_VERB 8

#ifndef OML_GLOBAL_LEVEL
#  define OML_GLOBAL_LEVEL OML_LEVEL_DEBUG
#endif

#ifndef OML_DEBUG_LEVEL
#  define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#endif

#define oml_fprintf fprintf
#define DBG_DEV stderr,
#define OML_FFLUSH fflush(stderr)

/** For internal use only	*/
#define oml_log_str(level,str,msg,args...)			\
do {								\
  int __level = (level);					\
  if (__level <= OML_DEBUG_LEVEL && __level <= OML_GLOBAL_LEVEL) {  \
    oml_fprintf(DBG_DEV "[%15s] " str msg "\n", __func__ , ##args); \
    OML_FFLUSH;							\
  }								\
} while (0)

/** Evaluate cond and log a warning if it is not satisfied.
 *
 * Allows to specify a custom printf-like format message with arguments.
 * Condition is always evaluated, independently of debug settings.
 */
#define oml_chk(cond,msg,args...) do {				\
  if (!(cond))							\
    oml_log_str(OML_LEVEL_WARN,   "<WRN> ",msg,##args);	\
} while (0)

/** Evaluate cond and log a warning if it is not satisfied.
 *
 * Allows to specify a custom printf-like format message with arguments.
 * Condition is always evaluated, independently of debug settings.
 */
#define oml_chk_go(cond,lab,msg,args...) do {		\
  if (!(cond)) {						\
    oml_log_str(OML_LEVEL_WARN,   "<WRN> ",msg,##args);	\
    goto lab;							\
  }								\
} while (0)


/** Log a message with a specified level	*/
#define oml_log(level,msg,args...) oml_log_str(level,"",msg,##args)

/** Log an error message	*/
#define oml_log_err(msg,args...) oml_log_str(OML_LEVEL_ERROR,   "<ERR> ",msg,##args)

/** Log a warning message	*/
#define oml_log_warn(msg,args...) oml_log_str(OML_LEVEL_WARN,  "<WRN> ",msg,##args)

/** Log an informative message	*/
#define oml_log_info(msg,args...) oml_log_str(OML_LEVEL_INFO,  "<INF> ",msg,##args)

/** Log a debugging message	*/
#define oml_log_debug(msg,args...) oml_log_str(OML_LEVEL_DEBUG, "<DBG> ",msg,##args)

/** Log a critical error: always reported.	*/
#define oml_log_crit(msg,args...)			\
do {							\
  oml_fprintf(DBG_DEV "[%15s] <CRIT> " msg "\n", __func__ , ##args); \
  OML_FFLUSH;						\
} while (0)

#endif
