#ifndef __OML_LIST_FIND_H__
#  define __OML_LIST_FIND_H__

#include "oml_list.h"
#include "oml_common.h"

#define oml_list_find_in_eq(this, value, p_it, p_end, f_eq) ({ \
  oml_rv __rv = OML_E_NOT_FOUND; \
  typeof(value) __value = (value); \
  while (oml_list_has_next((this), (p_it)) \
         && ! oml_list_iter_eq((this), (p_it), (p_end))) { \
    typeof(value) __val; \
    oml_list_get_next((this), (p_it), &__val); \
    if (f_eq(__val, __value)) { \
      __rv = OML_OK; \
      break; \
    } \
    oml_list_next((this), (p_it)); \
  } \
  __rv; \
})

#define oml_list_find_in(this, value, p_it, p_end) \
  oml_list_find_in_eq((this), (value), (p_it), (p_end), oml_default_eq)

#define oml_list_find_eq(this, value, p_it, f_eq) ({ \
  oml_rv __rv = OML_E_NOT_FOUND; \
  typeof(value) __value = (value); \
  for (oml_list_begin((this), (p_it)); \
       oml_list_has_next((this), (p_it)); \
       oml_list_next((this), (p_it)) \
  ) { \
    typeof(value) __val; \
    oml_list_get_next((this), (p_it), &__val); \
    if (f_eq(__val, __value)) { \
      __rv = OML_OK; \
      break; \
    } \
  } \
  __rv; \
})

#define oml_list_find_eq_param(this, value, p_it, f_eq, f_eq_param) ({ \
  oml_rv __rv = OML_E_NOT_FOUND; \
  typeof(value) __value = (value); \
  for (oml_list_begin((this), (p_it)); \
       oml_list_has_next((this), (p_it)); \
       oml_list_next((this), (p_it)) \
  ) { \
    oml_list_value_type(this) __val; \
    oml_list_get_next((this), (p_it), &__val); \
    if (f_eq(__val, __value, f_eq_param)) { \
      __rv = OML_OK; \
      break; \
    } \
  } \
  __rv; \
})

#define oml_list_find(this, value, p_it) \
  (oml_list_find_eq((this), (value), (p_it), oml_default_eq))

#endif
