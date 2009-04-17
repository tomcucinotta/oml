#ifndef __OML_SVECTOR_H__
#define __OML_SVECTOR_H__

/** @file
 **
 ** @brief A generic statically allocated vector, with optional bound checks.
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

/**
 ** @param size		Max. number of envectorable elements
 **/
#define oml_define_svector(value_type, size) \
  /** Array-based contained with optional bound checks **/ \
  typedef struct oml_svector_##value_type ##_##size { \
    value_type elems[size];    /**< Array of contained elems          */ \
    int num_elems;        /**< Current number of contained elems */ \
    int max_num_elems;    /**< Max number of elems  */ \
  } oml_svector_##value_type ##_##size; \
  typedef struct oml_svector_##value_type ##_##size ##_iterator { \
    /** Position of the elem to be returned by next oml_svector_next */ \
    int pos; \
  } oml_svector_##value_type ##_##size ##_iterator

/** Template-like type for a vector container */
#define oml_svector(value_type, size) \
  oml_svector_##value_type ##_##size

/** Template-like type for a vector iterator.
 ** 
 ** Each iterator tolerates extraction of already iterated elems
 ** and insertion of new elems during iteration.
 **/
#define oml_svector_iterator(value_type, size) \
  oml_svector_##value_type ##_##size ##_iterator

/** Initialize a vector specifying the max. number of envectorable elements.
 **
 ** @param vector		Pointer to a oml_svector instance
 **
 ** @return OML_OK or OML_E_NO_MEMORY
 **/
#define oml_svector_init(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->max_num_elems = sizeof((this)->elems) / sizeof((this)->elems[0]); \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Finalize a vector and deallocated associated resources. */
#define oml_svector_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Clear a vector contents, so that it will become empty. */
#define oml_svector_clear(this) ({ \
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
#define oml_svector_push_back(this, value) ({ \
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

/** Add a value at the begin of the vector.
 **
 ** After the insertion operation, the very first element
 ** a forward iteration retrieves is the just inserted one.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_svector_push_front(this, _value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    int _pos; \
    if ((this)->num_elems == (this)->max_num_elems) { \
      __rv = OML_E_FULL; \
      break; \
    } \
    for (_pos = (this)->num_elems; _pos > 0; --_pos) \
      (this)->elems[_pos] = (this)->elems[_pos - 1]; \
    (this)->elems[0] = _value; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

/** Read the value at the front of the vector, without removing it.
 **
 ** The element that is retrieved through this operation is the same
 ** that would be extracted through a oml_svector_pop operation.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_svector_front(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if ((p_value) != NULL)					 \
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
#define oml_svector_pop(this) ({ \
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
#define oml_svector_size(this) ((this)->num_elems)

/** Check if the vector is empty. **/
#define oml_svector_empty(this) ((this)->num_elems == 0)

/** Check if the vector is full. **/
#define oml_svector_full(this) ((this)->num_elems == (this)->max_num_elems)

/* FORWARD ITERATOR */

/** Get an iterator positioned on the first (earliest inserted)
 ** element of the vector, i.e. the earliest inserted element is
 ** the first one to be returned by the very next oml_svector_next
 ** call.
 **/
#define oml_svector_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (p_it)->pos = 0; \
  } while (0); \
  __rv; \
})

/** Check if we may call oml_svector_next() once again **/
#define oml_svector_has_value(this, p_it) \
  ((p_it)->pos != (this)->num_elems)

/** Move the iterator one element forward. */
#define oml_svector_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_svector_has_value((this), (p_it))) { \
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
#define oml_svector_value(this, p_it) ((this)->elems[(p_it)->pos])

/* REVERSE ITERATOR */

/** Get a reverse iterator positioned on the tail of the vector,
 ** i.e. the first element to be returned by the very next oml_svector_prev
 ** call is the same that would be returned with a forward iteration (made
 ** through oml_svector_begin / oml_svector_next.
 **/
#define oml_svector_end(this, p_it) \
  do { \
    (p_it)->pos = (this)->num_elems; \
  } while (0)

/** Check if the two iterators refer to the same position **/
#define oml_svector_iter_eq(this, p_it1, p_it2) \
  ((p_it1)->pos == (p_it2)->pos)

/** Check if we may call oml_svector_prev() once again **/
#define oml_svector_has_prev(this, p_it) \
  ((p_it)->pos != 0)

/** Retrieve (without removing) the prev element while iterating the vector,
 ** and update iterator position.
 **/
#define oml_svector_prev(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_svector_has_prev((this), (p_it))) { \
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
#define oml_svector_get_prev(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
     if (! oml_svector_has_prev((this), (p_it))) { \
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
 ** The result is the same as getting an iterator with oml_svector_begin
 ** and calling oml_svector_next a number of times equal to the specified
 ** position.
 ** 
 ** @param pos
 ** The position at which to initialize the iterator. Providing zero makes
 ** this operation equivalent to oml_svector_begin. Providing oml_svector_size(this)
 ** makes this operation equivalent to oml_svector_end.
 **/
#define oml_svector_at(this, p_it, pos) ({ \
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
