#ifndef __OML_PAIR_H__
#define __OML_PAIR_H__

/**
 * @file
 * 
 * @brief Generic pair.
 */

#include "oml_debug.h"
#include "oml_common.h"

/** Define pair related types: use this macro only once per compilation unit */
#define oml_define_pair(first_type, second_type) \
  typedef struct oml_pair_##first_type ##_##second_type { \
    first_type first; \
    second_type second; \
  } oml_pair_##first_type ##_##second_type

/* /\** Define pair related types: use this macro only once per compilation unit *\/ */
/* #define oml_define_pair(first_type, second_type) \ */
/*   __oml_define_pair(first_type, second_type) */

#define __oml_pair(__first_type, __second_type) \
  oml_pair_##__first_type ##_##__second_type

/** Template-like type for a pair container */
#define oml_pair(first_type, second_type) \
  __oml_pair(first_type, second_type)

/** Initialize a pair. **/
#define oml_pair_init(this, _first, _second) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->first = _first; \
    (this)->second = _second; \
  } while (0); \
  __rv; \
})

/** Access (r/w) the first element in the pair */
#define oml_pair_first(this) \
  ((this)->first)

/** Access (r/w) the second element in the pair */
#define oml_pair_second(this) \
  ((this)->second)

#endif /* __OML_PAIR_H__ */
