#ifndef __OML_VECTOR_H__
#define __OML_VECTOR_H__

/** @file
 **
 ** @brief A generic vector, with optional bound checks.
 **
 ** All vector operations are declared as static inline, and their
 ** implementation is in the header file itself, so to allow the
 ** compiler to optimize as much as possible. It is possible to enable
 ** or disable various checks on the consistency of each opearation
 ** with respect to the internal status of the vector object. This is
 ** useful for debugging purposes.
 **/

#include "oml_debug.h"
#include "oml_malloc.h"

#define oml_define_vector(value_type) \
  /** Array-based contained with optional bound checks **/ \
  typedef struct oml_vector_##value_type { \
    value_type *elems;    /**< Array of contained elems          */ \
    int num_elems;        /**< Current number of contained elems */ \
    int max_num_elems;    /**< Max number of elems  */ \
  } oml_vector_##value_type; \
  typedef struct oml_vector_##value_type ##_iterator { \
    /** Position of the elem to be returned by next oml_vector_next */ \
    int pos; \
  } oml_vector_##value_type ##_iterator

/** Template-like type for a vector container */
#define oml_vector(value_type) \
  oml_vector_##value_type

/** Template-like type for a vector iterator.
 ** 
 ** Each iterator tolerates extraction of already iterated elems
 ** and insertion of new elems during iteration.
 **/
#define oml_vector_iterator(value_type) \
  oml_vector_##value_type ##_iterator

/** Initialize a vector specifying the max. number of envectorable elements.
 **
 ** @param vector		Pointer to a oml_vector instance
 ** @param max_num_elems	Max. number of envectorable elements
 **
 ** @return OML_OK or OML_E_NO_MEMORY
 **/
#define oml_vector_init(this, N) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->elems = oml_malloc(sizeof((this)->elems[0]) * N); \
    if ((this)->elems == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    (this)->max_num_elems = N; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Finalize a vector and deallocated associated resources. */
#define oml_vector_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_free((this)->elems); \
    (this)->elems = NULL; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Clear a vector contents, so that it will become empty. */
#define oml_vector_clear(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Add a value into the (tail of the) vector.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_vector_push(this, value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == (this)->max_num_elems) { \
      __rv = OML_E_FULL; \
      break; \
    } \
    (this)->elems[(this)->num_elems] = value; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

/** Read the value at the front of the vector, without removing it.
 **
 ** The element that is retrieved through this operation is the same
 ** that would be extracted through a oml_vector_pop operation.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vector_front(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if (p_value) \
      *(p_value) = (this)->elems[(this)->num_elems - 1]; \
  } while (0); \
  __rv; \
})

/** Remove a value from (the tail of) the vector and return it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the popped element,
 **   or NULL if you don't need to know what has been extracted.
 **
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vector_pop(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    (this)->num_elems--; \
  } while (0); \
  __rv; \
})

/** Return the number of values into the vector. **/
#define oml_vector_size(this) ((this)->num_elems)

/** Check if the vector is empty. **/
#define oml_vector_empty(this) ((this)->num_elems == 0)

/** Check if the vector is full. **/
#define oml_vector_full(this) ((this)->num_elems == (this)->max_num_elems)

/* FORWARD ITERATOR */

/** Get an iterator positioned on the first (earliest inserted)
 ** element of the vector, i.e. the earliest inserted element is
 ** the first one to be returned by the very next oml_vector_next
 ** call.
 **/
#define oml_vector_begin(this, p_it) \
  do { \
    (p_it)->pos = 0; \
  } while (0)

/** Check if we may call oml_vector_next() once again **/
#define oml_vector_has_next(this, p_it) \
  ((p_it)->pos != (this)->num_elems))

/** Move the iterator one element forward. */
#define oml_vector_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_vector_has_next((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    (p_it)->pos++; \
  } while (0); \
  __rv; \
})

/** Retrieve the next element of a forward iteration, if any.
 **
 ** @return OML_E_NOT_FOUND if there is no next element in the iteration.
 **/
#define oml_vector_get_next(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
     if (! oml_vector_has_next((this), (p_it))) { \
       __rv = OML_E_NOT_FOUND; \
       break; \
     } \
     *(p_value) = (this)->elems[(p_it)->pos]; \
  } while (0); \
  __rv; \
})

/* REVERSE ITERATOR */

/** Get a reverse iterator positioned on the tail of the vector,
 ** i.e. the first element to be returned by the very next oml_vector_prev
 ** call is the same that would be returned with a forward iteration (made
 ** through oml_vector_begin / oml_vector_next.
 **/
#define oml_vector_end(this, p_it) \
  do { \
    (p_it)->pos = (this)->num_elems; \
  } while (0)

/** Check if we may call oml_vector_prev() once again **/
#define oml_vector_has_prev(this, p_it) \
  ((p_it)->pos != 0)

/** Retrieve (without removing) the prev element while iterating the vector,
 ** and update iterator position.
 **/
#define oml_vector_prev(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_vector_has_prev((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    (p_it)->pos--; \
  } while (0); \
  __rv; \
})

/** Retrieve the next element of a reverse iteration, if any.
 **
 ** @return OML_E_NOT_FOUND if there is no next element in the iteration.
 **/
#define oml_vector_get_prev(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
     if (! oml_vector_has_prev((this), (p_it))) { \
       __rv = OML_E_NOT_FOUND; \
       break; \
     } \
     *(p_value) = (this)->elems[(p_it)->pos - 1]; \
  } while (0); \
  __rv; \
})

/* RANDOM ACCESS */

/** Get an iterator located at the specified position in the vector.
 **
 ** The result is the same as getting an iterator with oml_vector_begin
 ** and calling oml_vector_next a number of times equal to the specified
 ** position.
 ** 
 ** @param pos
 ** The position at which to initialize the iterator. Providing zero makes
 ** this operation equivalent to oml_vector_begin. Providing oml_vector_size(this)
 ** makes this operation equivalent to oml_vector_end.
 **/
#define oml_vector_at(this, p_it, pos) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((pos) < 0 || (pos) >= (this)->num_elems) { \
      __rv = OML_E_INVALID_PARAM; \
      break; \
    } \
    (p_it)->pos = pos; \
  } while (0); \
  __rv; \
})

#endif
