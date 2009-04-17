#ifndef __OML_VECTOR_FIND_H__
#  define __OML_VECTOR_FIND_H__

#include "oml_vector.h"
#include "oml_common.h"

#define oml_vector_find_in_eq(this, value, p_it, p_end, f_eq) ({	\
      oml_rv __rv = OML_E_NOT_FOUND;					\
      typeof(value) __value = (value);					\
      while (oml_vector_has_value((this), (p_it))			\
	     && ! oml_vector_iter_eq((this), (p_it), (p_end))) {	\
	typeof(value) __val;						\
	oml_vector_get((this), (p_it), &__val);				\
	if (f_eq(__val, __value)) {					\
	  __rv = OML_OK;						\
	  break;							\
	}								\
	oml_vector_next((this), (p_it));				\
      }									\
      __rv;								\
    })

#define oml_vector_find_in(this, value, p_it, p_end) \
  oml_vector_find_in_eq((this), (value), (p_it), (p_end), oml_default_eq)

#define oml_vector_find_eq(this, value, p_it, f_eq) ({	\
      oml_rv __rv = OML_E_NOT_FOUND;			\
      typeof(value) __value = (value);			\
      for (oml_vector_begin((this), (p_it));		\
	   oml_vector_has_value((this), (p_it));	\
	   oml_vector_next((this), (p_it))		\
	   ) {						\
	typeof(value) __val;				\
	oml_vector_get((this), (p_it), &__val);		\
	if (f_eq(__val, __value)) {			\
	  __rv = OML_OK;				\
	  break;					\
	}						\
      }							\
      __rv;						\
    })

#define oml_vector_find_eq_param(this, value, p_it, f_eq, f_eq_param) ({ \
  oml_rv __rv = OML_E_NOT_FOUND; \
  typeof(value) __value = (value); \
  for (oml_vector_begin((this), (p_it)); \
       oml_vector_has_value((this), (p_it)); \
       oml_vector_next((this), (p_it)) \
  ) { \
    oml_vector_value_type(this) __val; \
    oml_vector_get((this), (p_it), &__val);	    \
    if (f_eq(__val, __value, f_eq_param)) { \
      __rv = OML_OK; \
      break; \
    } \
  } \
  __rv; \
})

#define oml_vector_find(this, value, p_it) \
  (oml_vector_find_eq((this), (value), (p_it), oml_default_eq))

#endif
