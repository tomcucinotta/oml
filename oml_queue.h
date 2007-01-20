#ifndef __OML_QUEUE_H__
#define __OML_QUEUE_H__

/** @file
 *
 * @brief Implementation inline di una coda (FIFO) di valori di tipo
 * oml_queue_elem_t.
 *
 * All queue operations are declared as static inline, and their
 * implementation is in the header file itself, so to allow the
 * compiler to optimize as much as possible. It is possible to enable
 * or disable various checks on the consistency of each opearation
 * with respect to the internal status of the queue object. This is
 * useful for debugging purposes.
 */

#include "oml_debug.h"
#include "oml_malloc.h"

#define oml_define_queue(value_type) \
  typedef struct oml_queue_##value_type { \
    value_type *elems;    /**< Array of enqueued elems          */ \
    int num_elems;        /**< Current number of enqueued elems */ \
    int max_num_elems;    /**< Max number of enqueueable elems  */ \
    /** Pos. in data[] of next elem to insert  */ \
    int ins_pos; \
    /** Pos. in data[] of next elem to extract */ \
    int del_pos; \
  } oml_queue_##value_type; \
  typedef struct oml_queue_##value_type ##_iterator { \
    /** Position of the elem to be returned by next oml_queue_next */ \
    int pos; \
    /** Number of times oml_queue_next() has been called */ \
    int num_iterated; \
  } oml_queue_##value_type ##_iterator

/** Template-like type for a queue container */
#define oml_queue(value_type) \
  oml_queue_##value_type

/** Template-like type for a queue iterator.
 ** 
 ** Each iterator tolerates extraction of already iterated elems
 ** and insertion of new elems during iteration.
 **/
#define oml_queue_iterator(value_type) \
  oml_queue_##value_type ##_iterator

/** Initialize a queue specifying the max. number of enqueueable elements.
 **
 ** @param queue		Pointer to a oml_queue instance
 ** @param max_num_elems	Max. number of enqueueable elements
 **
 ** @return OML_OK or OML_E_NO_MEMORY
 **/
#define oml_queue_init(this, N) ({ \
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
#define oml_queue_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_free((this)->elems); \
    (this)->elems = NULL; \
  } while (0); \
  __rv; \
})

/** Enqueue a value into the (tail of the) queue.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_queue_push(this, value) ({ \
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

/** Read the value at the front of the queue, without removing it.
 **
 ** The element that is retrieved through this operation is the same
 ** that would be extracted through a oml_queue_pop operation.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_queue_front(this, p_value) ({ \
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

/** Dequeue a value from (the front of) the queue and return it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the popped element,
 **   or NULL if you don't need to know what has been extracted.
 **
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_queue_pop(this, p_value) ({ \
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

/** Return the number of values into the queue. **/
#define oml_queue_size(this) ((this)->num_elems)

/** Check if the queue is empty. **/
#define oml_queue_empty(this) ((this)->num_elems == 0)

/** Check if the queue is full. **/
#define oml_queue_full(this) ((this)->num_elems == (this)->max_num_elems)

/* Queue Iterator */

/** Get an iterator positioned on the first (earliest inserted)
 ** element of the queue, i.e. the earliest inserted element is
 ** the first one to be returned by the very next oml_queue_next
 ** call.
 **/
#define oml_queue_begin(this, p_it) \
  do { \
    (p_it)->pos = (this)->del_pos; \
    (p_it)->num_iterated = 0; \
  } while (0)

/** Check if we may call oml_queue_next() once again **/
#define oml_queue_has_next(this, p_it) \
  ( \
    ((p_it)->pos != (this)->ins_pos) \
    || (oml_queue_full(this) && (p_it)->num_iterated == 0) \
  )

/** Retrieve (without removing) the next element while iterating the queue */
#define oml_queue_next(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_queue_has_next((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_value) = (this)->elems[(p_it)->pos]; \
    (p_it)->pos = ((p_it)->pos + 1) % (this)->max_num_elems; \
    (p_it)->num_iterated++; \
  } while (0); \
  __rv; \
})

#endif
