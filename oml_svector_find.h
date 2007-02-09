#ifndef __OML_VECTOR_FIND_H__
#  define __OML_VECTOR_FIND_H__

#include "oml_svector.h"
#include "oml_common.h"

#define oml_svector_find_in_eq(this, value, p_it, p_end, f_eq) ({ \
  oml_rv __rv = OML_E_NOT_FOUND; \
  typeof(value) __value = (value); \
  while (oml_svector_has_next((this), (p_it)) \
         && ! oml_svector_iter_eq((this), (p_it), (p_end))) { \
    typeof(value) __val; \
    oml_svector_get_next((this), (p_it), &__val); \
    if (f_eq(__val, __value)) { \
      __rv = OML_OK; \
      break; \
    } \
    oml_svector_next((this), (p_it)); \
  } \
  __rv; \
})

#define oml_svector_find_in(this, value, p_it, p_end) \
  oml_svector_find_in_eq((this), (value), (p_it), (p_end), oml_default_eq)

#define oml_svector_find_eq(this, value, p_it, f_eq) ({ \
  oml_rv __rv = OML_E_NOT_FOUND; \
  typeof(value) __value = (value); \
  for (oml_svector_begin((this), (p_it)); \
       oml_svector_has_next((this), (p_it)); \
       oml_svector_next((this), (p_it)) \
  ) { \
    typeof(value) __val; \
    oml_svector_get_next((this), (p_it), &__val); \
    if (f_eq(__val, __value)) { \
      __rv = OML_OK; \
      break; \
    } \
  } \
  __rv; \
})

#define oml_svector_find(this, value, p_it) \
  (oml_svector_find_eq((this), (value), (p_it), oml_default_eq))

#endif
