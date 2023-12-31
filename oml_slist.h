#ifndef __OML_LIST_H__
#define __OML_LIST_H__

/** @file
 ** 
 ** @brief Generic linked list.
 **/

#include "oml_debug.h"
#include "oml_malloc.h"

#define oml_define_list(value_type) \
  typedef struct oml_list_node_##value_type { \
    value_type value; \
    struct oml_list_node_##value_type *p_next; \
  } oml_list_node_##value_type; \
  typedef oml_list_node_##value_type *oml_list_iterator_##value_type; \
  typedef struct oml_list_##value_type { \
    oml_list_node(value_type) *p_head; /* First list node */ \
    int num_elems; \
  } oml_list_##value_type

#define oml_list_node(value_type) \
  oml_list_node_##value_type

#define oml_list_iterator(value_type) \
  oml_list_iterator_##value_type

#define oml_list_value_type(this) typeof((this)->p_head->value)

#define oml_list(value_type) \
  oml_list_##value_type

#define oml_list_init(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    (this)->p_head = NULL; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Inserts _value in place of *it, pushing *it at the left child of _value */
#define oml_list_push_front(this, _value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof((this)->p_head) __p_node = oml_malloc(sizeof(typeof(*(this)->p_head))); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->p_next = (this)->p_head; \
    __p_node->value = (_value); \
    (this)->p_head = __p_node; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

/** Retrieves the value from the head of the list, if any. */
#define oml_list_front(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (oml_list_empty(this)) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if ((p_value) != NULL) \
      *(p_value) = (this)->p_head->value; \
  } while (0); \
  __rv; \
})

/** Removes a value from the head of the list */
#define oml_list_pop_front(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof((this)->p_head) __p_node; \
    if (oml_list_empty(this)) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    __p_node = (this)->p_head; \
    (this)->p_head = __p_node->p_next; \
    (this)->num_elems--; \
    oml_free(__p_node); \
  } while (0); \
  __rv; \
})

#define oml_list_size(this) ((this)->num_elems)

#define oml_list_empty(this) ((this)->num_elems == 0)

#define oml_list_clear(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    while ((this)->p_head != NULL) { \
      typeof((this)->p_head) __p_node = (this)->p_head; \
      (this)->p_head = (this)->p_head->p_next; \
      oml_free(__p_node); \
    } \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_list_cleanup(this) \
  oml_list_clear(this)

/* FORWARD ITERATOR */

/** Get an iterator positioned on the head element
 **/
#define oml_list_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *(p_it) = (this)->p_head; \
  } while (0); \
  __rv; \
})

/** Check if we may call oml_list_next() once again **/
#define oml_list_has_value(this, p_it) \
  (*(p_it) != NULL)

/** Move the iterator one element forward. */
#define oml_list_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_list_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_it) = (*(p_it))->p_next; \
  } while (0); \
  __rv; \
})

/** Retrieve the next element of a forward iteration, if any.
 **
 ** @return OML_E_NOT_FOUND if there is no next element in the iteration.
 **/
#define oml_list_get_next(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_list_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_value) = (*(p_it))->value; \
  } while (0); \
  __rv; \
})

/** Direct (L-assignable) reference to the value referenced by p_it.
 **
 ** @note If the iterator is not valid, behaviour is unspecified.
 **/
#define oml_list_value(this, p_it) ((*(p_it))->value)

/** Get an iterator positioned past the last element
 **/
#define oml_list_end(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *(p_it) = NULL; \
  } while (0); \
  __rv; \
})

/** Check if the two iterators refer to the same position **/
#define oml_list_iter_eq(this, p_it1, p_it2) \
  ((*(p_it1)) == (*(p_it2)))

#endif /* __OML_LIST_H__ */
