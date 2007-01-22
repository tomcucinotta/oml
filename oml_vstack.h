#ifndef __OML_VSTACK_H__
#define __OML_VSTACK_H__

/** @file
 **
 ** @brief A generic vector-based stack, with optional bound checks.
 **/

#include "oml_vector.h"

#define oml_define_vstack(value_type) oml_define_vstack(value_type)

/** Template-like type for a vstack container */
#define oml_vstack(value_type) oml_vstack(value_type)

/** Template-like type for a vstack iterator.
 ** 
 ** Each iterator tolerates extraction of already iterated elems
 ** and insertion of new elems during iteration.
 **/
#define oml_vstack_iterator(value_type) oml_vector_iterator(value_type)

/** Initialize a vstack specifying the max. number of elements.
 **
 ** @param vstack		Pointer to a oml_vstack instance
 ** @param max_num_elems	Max. number of elements
 **
 ** @return OML_OK or OML_E_NO_MEMORY
 **/
#define oml_vstack_init(this, N) oml_vector_init(this, N)

/** Finalize a vstack and deallocated associated resources. */
#define oml_vstack_cleanup(this) oml_vector_cleanup(this)

/** Push a value into the the vstack.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_vstack_push(this, value) oml_vector_push(this, value)

/** Read from the vstack the last pushed value, without removing it.
 **
 ** The element that is retrieved through this operation is the same
 ** that would be extracted through a oml_vstack_pop operation.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vstack_front(this, p_value) oml_vector_front(this, p_value)

/** Pop from the vstack the last pushed value and return it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the popped element,
 **   or NULL if you don't need to know what has been extracted.
 **
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_vstack_pop(this) oml_vector_pop(this)

/** Return the number of values into the vstack. **/
#define oml_vstack_size(this) oml_vector_size(this)

/** Check if the vstack is empty. **/
#define oml_vstack_empty(this) oml_vector_empty(this)

/** Check if the vstack is full. **/
#define oml_vstack_full(this) oml_vector_full(this)

/* VStack Iterator */

/** Get an iterator positioned on the top of the vstack, i.e.
 ** the latest inserted element is the first one to be returned
 ** by the very next call to oml_vstack_next.
 **/
#define oml_vstack_begin(this, p_it) oml_vector_end((this), (p_it))

/** Check if we may call oml_vstack_next() once again **/
#define oml_vstack_has_next(this, p_it) oml_vector_has_prev((this), (p_it))

/** Move the iterator one element down the vstack. */
#define oml_vstack_next(this, p_it) oml_vector_prev((this), (p_it))

/** Retrieve the next element in the vstack iteration, if any.
 **
 ** @return OML_E_NOT_FOUND if there is no next element in the iteration.
 **/
#define oml_vstack_get_next(this, p_it, p_value) oml_vector_get_prev((this), (p_it), (p_value))

#endif
