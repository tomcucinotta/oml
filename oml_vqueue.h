#ifndef __OML_VQUEUE_H__
#define __OML_VQUEUE_H__

/** @file
 *
 * @brief Vector-based queue implementation.
 *
 * All vqueue operations are declared as static inline, and their
 * implementation is in the header file itself, so to allow the
 * compiler to optimize as much as possible. It is possible to enable
 * or disable various checks on the consistency of each opearation
 * with respect to the internal status of the vqueue object. This is
 * useful for debugging purposes.
 */

#include "oml_debug.h"
#include "oml_malloc.h"

#define oml_define_vqueue(value_type) \
  typedef struct oml_vqueue_##value_type { \
    value_type *elems;    /**< Array of envqueued elems          */ \
    int num_elems;        /**< Current number of envqueued elems */ \
    int max_num_elems;    /**< Max number of envqueueable elems  */ \
    /** Pos. in data[] of next elem to insert  */ \
    int ins_pos; \
    /** Pos. in data[] of next elem to extract */ \
    int del_pos; \
  } oml_vqueue_##value_type; \
  typedef struct oml_vqueue_##value_type ##_iterator { \
    /** Position of the elem to be returned by next oml_vqueue_next */ \
    int pos; \
    /** Number of times oml_vqueue_next() has been called */ \
    int num_iterated; \
  } oml_vqueue_##value_type ##_iterator

/** Template-like type for a vqueue container */
#define oml_vqueue(value_type) \
  oml_vqueue_##value_type

/** Template-like type for a vqueue iterator.
 ** 
 ** Each iterator tolerates extraction of already iterated elems
 ** and insertion of new elems during iteration.
 **/
#define oml_vqueue_iterator(value_type) \
  oml_vqueue_##value_type ##_iterator

/** Initialize a vqueue specifying the max. number of envqueueable elements.
 **
 ** @param vqueue		Pointer to a oml_vqueue instance
 ** @param max_num_elems	Max. number of envqueueable elements
 **
 ** @return OML_OK or OML_E_NO_MEMORY
 **/
#define oml_vqueue_init(this, N) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->elems = oml_malloc(sizeof((this)->elems[0]) * N); \
    if ((this)->elems == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    (this)->max_num_elems = N; \
    (this)->num_elems = 0; \
    (this)->ins_pos = 0; \
    (this)->del_pos = 0; \
  } while (0); \
  __rv; \
})

/** Distrugge una coda e le risorse associate	*/
#define oml_vqueue_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_free((this)->elems); \
    (this)->elems = NULL; \
  } while (0); \
  __rv; \
})

/** Envqueue a value into the (tail of the) vqueue.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_vqueue_push(this, value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == (this)->max_num_elems) { \
      __rv = OML_E_FULL; \
      break; \
    } \
    (this)->elems[(this)->ins_pos] = value; \
    (this)->ins_pos = ((this)->ins_pos + 1) % (this)->max_num_elems; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

/** Read the value at the front of the vqueue, without removing it.
 **
 ** The element that is retrieved through this operation is the same
 ** that would be extracted through a oml_vqueue_pop operation.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vqueue_front(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if (p_value) \
      *(p_value) = (this)->elems[(this)->del_pos]; \
  } while (0); \
  __rv; \
})

/** Devqueue a value from (the front of) the vqueue and return it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the popped element,
 **   or NULL if you don't need to know what has been extracted.
 **
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vqueue_pop(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if (p_value) \
      *(p_value) = (this)->elems[(this)->del_pos]; \
    (this)->del_pos = ((this)->del_pos + 1) % (this)->max_num_elems; \
    (this)->num_elems--; \
  } while (0); \
  __rv; \
})

/** Return the number of values into the vqueue. **/
#define oml_vqueue_size(this) ((this)->num_elems)

/** Check if the vqueue is empty. **/
#define oml_vqueue_empty(this) ((this)->num_elems == 0)

/** Check if the vqueue is full. **/
#define oml_vqueue_full(this) ((this)->num_elems == (this)->max_num_elems)

/* Vqueue Iterator */

/** Get an iterator positioned on the first (earliest inserted)
 ** element of the vqueue, i.e. the earliest inserted element is
 ** the first one to be returned by the very next oml_vqueue_next
 ** call.
 **/
#define oml_vqueue_begin(this, p_it) \
  do { \
    (p_it)->pos = (this)->del_pos; \
    (p_it)->num_iterated = 0; \
  } while (0)

/** Check if we may call oml_vqueue_next() once again **/
#define oml_vqueue_has_value(this, p_it) \
  ( \
    ((p_it)->pos != (this)->ins_pos) \
    || (oml_vqueue_full(this) && (p_it)->num_iterated == 0) \
  )

/** Retrieve (without removing) the next element while iterating the vqueue */
#define oml_vqueue_next(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_vqueue_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_value) = (this)->elems[(p_it)->pos]; \
    (p_it)->pos = ((p_it)->pos + 1) % (this)->max_num_elems; \
    (p_it)->num_iterated++; \
  } while (0); \
  __rv; \
})

/** Retrieve the element that would be returned by the next call to oml_vqueue_next, if any.
 **
 ** @return OML_E_NOT_FOUND if there is no next element in the iteration.
 **/
#define oml_vqueue_get(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
     if (! oml_vqueue_has_value((this), (p_it))) { \
       __rv = OML_E_NOT_FOUND; \
       break; \
     } \
     *(p_value) = (this)->elems[(p_it)->pos]; \
  } while (0); \
  __rv; \
})

#endif
