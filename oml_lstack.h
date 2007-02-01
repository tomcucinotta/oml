#ifndef __OML_LSTACK_H__
#define __OML_LSTACK_H__

/** @file
 **
 ** @brief A generic lsit-based stack.
 **/

#include "oml_list.h"

#define oml_define_lstack(value_type) oml_define_list(value_type)

/** Template-like type for a lstack container */
#define oml_lstack(value_type) oml_list(value_type)

/** Template-like type for a lstack iterator.
 ** 
 ** Each iterator tolerates extraction of already iterated elems
 ** and insertion of new elems during iteration.
 **/
#define oml_lstack_iterator(value_type) oml_list_iterator(value_type)

/** Initialize a lstack specifying the max. number of elements.
 **
 ** @param lstack		Pointer to a oml_lstack instance
 **
 ** @return OML_OK or OML_E_NO_MEMORY
 **/
#define oml_lstack_init(this) oml_list_init(this)

/** Finalize a lstack and deallocated associated resources. */
#define oml_lstack_cleanup(this) oml_list_cleanup(this)

/** Push a value into the the lstack.
 **
 ** @return OML_OK or OML_E_FULL
 **/
#define oml_lstack_push(this, value) oml_list_add_head((this), (value))

/** Read from the lstack the last pushed value, without removing it.
 **
 ** The element that is retrieved through this operation is the same
 ** that would be extracted through a oml_lstack_pop operation.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the read value,
 **   or NULL if you don't need to know what has been extracted.
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_lstack_front(this, p_value) oml_list_head((this), (p_value))

/** Pop from the lstack the last pushed value and return it.
 **
 ** @param p_value
 **   Pointer to a variable that will contain the popped element,
 **   or NULL if you don't need to know what has been extracted.
 **
 ** @return
 **   OML_OK or OML_E_EMPTY
 **/
#define oml_lstack_pop(this) oml_list_del_head(this)

/** Return the number of values into the lstack. **/
#define oml_lstack_size(this) oml_list_size(this)

/** Check if the lstack is empty. **/
#define oml_lstack_empty(this) oml_list_empty(this)

/* LStack Iterator */

/** Get an iterator positioned on the top of the lstack, i.e.
 ** the latest inserted element is the first one to be returned
 ** by the very next call to oml_lstack_next.
 **/
#define oml_lstack_begin(this, p_it) oml_list_begin((this), (p_it))

/** Check if we may call oml_lstack_next() once again **/
#define oml_lstack_has_next(this, p_it) oml_list_has_next((this), (p_it))

/** Move the iterator one element down the lstack. */
#define oml_lstack_next(this, p_it) oml_list_next((this), (p_it))

/** Retrieve the next element in the lstack iteration, if any.
 **
 ** @return OML_E_NOT_FOUND if there is no next element in the iteration.
 **/
#define oml_lstack_get_next(this, p_it, p_value) oml_list_get_next((this), (p_it), (p_value))

#endif
