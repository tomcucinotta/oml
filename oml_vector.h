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

#define oml_vector_value_type(this) typeof((this)->elems[0])

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

/** Add a value at the end of the vector.
 **
 ** After the insertion operation, the very last element
 ** a forward iteration retrieves is the just inserted one.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_vector_push_back(this, value) ({ \
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
#define oml_vector_push_front(this, _value) ({ \
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

/** Read the value at the end of the vector, without removing it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vector_get_back(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if ((p_value) != NULL) \
      *(p_value) = (this)->elems[(this)->num_elems - 1]; \
  } while (0); \
  __rv; \
})

#define oml_vector_back(this) ((this)->elems[(this)->num_elems - 1])

/** Remove a value from the tail of the vector and return it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the popped element,
 **   or NULL if you don't need to know what has been extracted.
 **
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vector_pop_back(this) ({ \
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

/** Remove a value from the head of the vector and return it.
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
 ** the first one to be returned by the very next oml_vector_get_next
 ** call.
 **/
#define oml_vector_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (p_it)->pos = 0; \
  } while (0); \
  __rv; \
})

/** Check if we may call oml_vector_get() or oml_vector_value() **/
#define oml_vector_has_value(this, p_it) \
  (((p_it)->pos >= 0) && ((p_it)->pos < (this)->num_elems))

/** Check if iterator is at end (past the last element) **/
#define oml_vector_at_end(this, p_it) \
  ((p_it)->pos == (this)->num_elems)

/** Check if iterator is at begin (on the first element or no-valued) **/
#define oml_vector_at_begin(this, p_it) \
  ((p_it)->pos == 0)

/** Move the iterator one element forward. */
#define oml_vector_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_log_debug("pos: %d", (p_it)->pos); \
    if (! oml_vector_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    (p_it)->pos++; \
    oml_log_debug("pos: %d", (p_it)->pos); \
  } while (0); \
  __rv; \
})

/** Move the iterator n elements forward. */
#define oml_vector_next_n(this, p_it, n) ({		\
  oml_rv __rv = OML_OK;					\
  int __i;						\
  int __n = (n);					\
  for (__i = 0; __i < __n && __rv == OML_OK; ++__i)	\
    __rv = oml_vector_next((this), (p_it));		\
  __rv;							\
})

/** Retrieve the next element of a forward iteration, if any. **/
#define oml_vector_value(this, p_it) ((this)->elems[(p_it)->pos])

/** Retrieve the first element of a forward iteration, if any. **/
#define oml_vector_front(this) ((this)->elems[0])

/** Retrieve and store in p_value the element referenced to by the iterator p_it.
 **
 ** @return
 ** OML_E_NOT_FOUND if the iterator is past the last element in the vector.
 **
 ** @see oml_vector_value()
 **/
#define oml_vector_get(this, p_it, p_value) ({			\
      oml_rv __rv = OML_OK;					\
      do {							\
	if (oml_vector_has_value((this), (p_it)))		\
	  *(p_value) = oml_vector_value((this), (p_it));	\
	else							\
	  __rv = OML_E_NOT_FOUND;				\
      } while (0);						\
      __rv;							\
    })

/** Insert an element at the specified position in the vector.
 **
 ** All elements in the vector, including the one that would
 ** be returned through oml_vector_get() before the insertion,
 ** are shifted one position forward in the vector.
 ** After the insertion operation, an iteration up to the end,
 ** through the same iterator, would retrieve the just inserted
 ** element first, then all the remaining elements.
 **/
#define oml_vector_insert(this, p_it, value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
     int pos; \
     if ((this)->num_elems == (this)->max_num_elems)) { \
       __rv = OML_E_FULL; \
       break; \
     } \
     for (pos = (this)->num_elems; pos > (p_it)->pos; --pos) \
       (this)->elems[pos] = (this)->elems[pos - 1]; \
     (this)->elems[pos] = value; \
  } while (0); \
  __rv; \
})

/* REVERSE ITERATOR */

/*
for (it = v.begin(); it != v.end(); ++it)
  cout << *it;

for (oml_vector_begin(&c, &it);
     ! oml_vector_at_end(&c, &it); // or has_value (less efficient)
     oml_vector_next(&c, &it))
  printf("%d", oml_vector_get(&c, &it));

it = v.end();
while (it != v.begin()) {
  --it;
  cout << *it;
}

oml_vector_end(&c, &it);
oml_vector_begin(&c, &begin);
while (oml_vector_iter_ne(&c, &it, &begin)) {
  oml_vector_prev(&c, &it);
  printf("%d", oml_vector_get(&c, &it));
}

// Shorthand:

oml_vector_end(&c, &it);
while (! oml_vector_at_begin(&c, &it, &begin)) {
  oml_vector_prev(&c, &it);
  printf("%d", oml_vector_get(&c, &it));
}

*/

/** Get a reverse iterator positioned on the tail of the vector,
 ** i.e. the first element to be returned by the very next oml_vector_get_prev
 ** call is the same that would be returned as last during a forward iteration
 ** (made through oml_vector_begin / oml_vector_next).
 **/
#define oml_vector_end(this, p_it) \
  do { \
    (p_it)->pos = (this)->num_elems; \
  } while (0)

/** Check if the two iterators refer to the same position **/
#define oml_vector_iter_eq(this, p_it1, p_it2) \
  ((p_it1)->pos == (p_it2)->pos)

/** Retrieve (without removing) the prev element while iterating the vector,
 ** and update iterator position.
 **/
#define oml_vector_prev(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_vector_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    (p_it)->pos--; \
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
#define oml_vector_seek(this, p_it, pos) ({ \
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
