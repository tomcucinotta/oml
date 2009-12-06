#ifndef __OML_SET_H__
#define __OML_SET_H__

/** @file
 ** 
 ** @brief Generic hashset.
 ** 
 ** @note
 ** If you need to hash keys with anything different from the oml_default_hash()
 ** function, or you need to compare elements with anything from the oml_default_eq()
 ** function, then...
 **/

#include "oml_debug.h"
#include "oml_malloc.h"
#include "oml_common.h"
#include "oml_list.h"
#include "oml_list_find.h"
#include "oml_dump.h"

/** Define set related types: use this macro only once per compilation unit */
#define oml_define_set(value_type) \
  oml_define_list(value_type); \
  typedef struct oml_set_iterator_##value_type { \
    /** Position in elems **/ \
    int pos; \
    /** Position in the list elems[pos] **/ \
    oml_list_iterator(value_type) it; \
  } oml_set_iterator_##value_type; \
  typedef struct oml_set_##value_type { \
    oml_list(value_type) *elems; \
    int num_elems; \
    int num_positions; \
    /** Iterator type stub **/ \
    oml_list_iterator(value_type) *p_it; \
  } oml_set_##value_type

/** Template-like type for a set container */
#define oml_set(value_type) \
  oml_set_##value_type

/** Template-like type for a set iterator */
#define oml_set_iterator(value_type) \
  oml_set_iterator_##value_type

/** Initialize an empty set with a hashtable size of 2 ^ N **/
#define oml_set_init(this, N) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    int _pos; \
    (this)->elems = oml_malloc(sizeof((this)->elems[0]) << N); \
    if ((this)->elems == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    (this)->num_positions = 1 << N; \
    for (_pos = 0; _pos < (this)->num_positions; _pos++) \
      oml_list_init(&(this)->elems[_pos]);		 \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_set_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    int _pos; \
    for (_pos = 0; _pos < (this)->num_positions; _pos++) \
      oml_list_cleanup(&(this)->elems[_pos]);		 \
    oml_free((this)->elems); \
    (this)->num_elems = (this)->num_positions = 0; \
  } while (0); \
  __rv; \
})

#define oml_value_eq(n1, n2, f_value_eq) (printf("Comparing %d and %d\n", (n1), (n2)), f_value_eq((n1), (n2)))

/**
 ** Add the element _value to the set.
 **
 ** @note Adding a second time the same value to the set has no effect.
 **/
#define oml_set_add_eq(this, _value, _op_value_eq) ({	\
  oml_rv __rv = OML_OK; \
  do { \
    typeof(_value) __set_value = (_value); \
    int _set_pos = oml_default_hash(__set_value) % ((this)->num_positions); \
    typeof(*((this)->p_it)) it; \
    __rv = oml_list_find_eq(&((this)->elems[_set_pos]), __set_value, &it, _op_value_eq); \
    if (__rv == OML_E_NOT_FOUND) { \
      __rv = oml_list_push_front(&((this)->elems[_set_pos]), __set_value); \
      if (__rv != OML_OK) \
        break; \
      oml_list_begin(&((this)->elems[_set_pos]), &it); \
      (this)->num_elems++;			       \
    } \
  } while (0); \
  __rv; \
})

/** Add the element _value to the set. **/
#define oml_set_add(this, _value) \
  oml_set_add_eq((this), (_value), oml_default_eq)

#define oml_set_del_eq(this, _value, _op_value_eq) ({   \
  oml_rv __rv = OML_OK; \
  do { \
    typeof(_value) __set_value = (_value); \
    int _set_pos = oml_default_hash(__set_value) % ((this)->num_positions); \
    __rv = oml_list_del(&((this)->elems[_set_pos]), __set_value); \
    if (__rv == OML_OK)						  \
      (this)->num_elems--;					  \
  } while (0); \
  __rv; \
})

#define oml_set_del(this, _value) \
  oml_set_del_eq((this), (_value), oml_default_eq)

/** Return the current size of the set */
#define oml_set_size(this) ((this)->num_elems)

#define oml_set_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (p_it)->pos = 0; \
    while ( ((p_it)->pos < (this)->num_positions) && oml_list_empty(&(this)->elems[(p_it)->pos]) ) \
      (p_it)->pos++; \
    if ((p_it)->pos < (this)->num_positions) \
      oml_list_begin(&(this)->elems[(p_it)->pos], &(p_it)->it); \
  } while (0); \
  __rv; \
})

#define oml_set_has_value(this, p_it) \
  ( ((p_it)->pos < (this)->num_positions) && oml_list_has_value(&(this)->elems[(p_it)->pos], &(p_it)->it) )

#define oml_set_get(this, p_it, p_val) ({ \
   oml_rv __rv = OML_OK; \
   do { \
     if ( ((p_it)->pos >= (this)->num_positions) || ! oml_list_has_value(&(this)->elems[(p_it)->pos], &(p_it)->it) ) { \
       __rv = OML_E_NOT_FOUND; \
       break; \
     } \
     oml_list_get(&(this)->elems[(p_it)->pos], &(p_it)->it, (p_val)); \
   } while (0); \
   __rv; \
})

#define oml_set_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ( ((p_it)->pos >= (this)->num_positions) || ! oml_list_has_value(&(this)->elems[(p_it)->pos], &(p_it)->it) ) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    oml_list_next(&(this)->elems[(p_it)->pos], &(p_it)->it); \
    if (! oml_list_has_value(&(this)->elems[(p_it)->pos], &(p_it)->it)) { \
      do { \
        (p_it)->pos++; \
      } while ( ((p_it)->pos < (this)->num_positions) && oml_list_empty(&(this)->elems[(p_it)->pos]) ); \
      if ((p_it)->pos < (this)->num_positions) \
        oml_list_begin(&(this)->elems[(p_it)->pos], &(p_it)->it); \
    } \
  } while (0); \
  __rv; \
})

#endif /* __OML_SET_H__ */
