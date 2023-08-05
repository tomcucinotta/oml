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
    struct oml_list_node_##value_type *p_next; \
    value_type value; \
  } oml_list_node_##value_type; \
  typedef oml_list_node_##value_type *oml_list_iterator_##value_type; \
  typedef struct oml_list_##value_type { \
    oml_list_node(value_type) *p_head; /* First list node */ \
    oml_list_node(value_type) *p_tail; /* Last list node */ \
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
    (this)->p_tail = NULL; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_list_push_front(this, _value) ({ \
  oml_rv __rv = OML_OK; \
  typeof(_value) __value = (_value);	\
  do { \
    typeof((this)->p_head) __p_node = oml_malloc(sizeof(typeof(*(this)->p_head))); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->p_next = (this)->p_head; \
    __p_node->value = __value; \
    (this)->p_head = __p_node; \
    if ((this)->p_tail == NULL) \
      (this)->p_tail = __p_node; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

#define oml_list_push_back(this, _value) ({ \
  oml_rv __rv = OML_OK; \
  typeof(_value) __value = (_value);		\
  do { \
    typeof((this)->p_head) __p_node = oml_malloc(sizeof(typeof(*(this)->p_head))); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->value = __value; \
    __p_node->p_next = NULL; \
    if ((this)->p_head == NULL) { \
      (this)->p_head = __p_node; \
    } else { \
      (this)->p_tail->p_next = __p_node; \
    } \
    (this)->p_tail = __p_node; \
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
    if ((p_value) != NULL)			  \
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
    if ((this)->p_tail == __p_node) \
      (this)->p_tail = NULL; \
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
    (this)->p_tail = NULL; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_list_cleanup(this) \
  oml_list_clear(this)

/* FORWARD CONST ITERATOR */

/** Get an iterator positioned on the head element
 **/
#define oml_list_const_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *(p_it) = (this)->p_head; \
  } while (0); \
  __rv; \
})

/** Alias of oml_list_const_begin() */
#define oml_list_cbegin(this, p_it) \
  oml_list_const_begin((this), (p_it))

/** Check if the iterator is dereferenceable (i.e. we may call oml_list_get() or oml_list_value()). **/
#define oml_list_const_has_value(this, p_it) \
  (*(p_it) != NULL)

/** Move the iterator one element forward. */
#define oml_list_const_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_list_const_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_it) = (*(p_it))->p_next; \
  } while (0); \
  __rv; \
})

/** Retrieve and store in p_value the element referenced to by the iterator p_it.
 **
 ** @return OML_E_NOT_FOUND if the iterator is past the last element in the list.
 **/
#define oml_list_const_get(this, p_it, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_list_const_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_value) = (*(p_it))->value; \
  } while (0); \
  __rv; \
})

/** Direct reference to the value referenced by p_it.
 **
 ** @note If the iterator is not valid, behaviour is unspecified.
 **/
#define oml_list_const_value(this, p_it) ((*(p_it))->value)

/** Get an iterator positioned past the last element. **/
#define oml_list_const_end(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *(p_it) = NULL; \
  } while (0); \
  __rv; \
})

/** Check if the two iterators refer to the same position **/
#define oml_list_const_iter_eq(this, p_it1, p_it2) \
  ((*(p_it1)) == (*(p_it2)))

/** Let the supplied iterator iterate over the entire collection **/
#define oml_list_const_foreach(this, p_it)	\
  for (oml_list_cbegin(this, p_it); oml_list_const_has_value(this, p_it); oml_list_const_next(this, p_it))

/* FORWARD R/W ITERATOR */

/** Get an iterator positioned on the head element **/
#define oml_list_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *(p_it) = NULL; \
  } while (0); \
  __rv; \
})

/** Check if the iterator is dereferenceable (i.e. we may call oml_list_get() or oml_list_value()). **/
#define oml_list_has_value(this, p_it) \
  ( (*(p_it) != NULL && (*(p_it))->p_next != NULL) || (*(p_it) == NULL && (this)->p_head != NULL) )

/** Move the iterator one element forward. */
#define oml_list_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_list_has_value((this), (p_it))) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    if (*(p_it) == NULL) \
      *(p_it) = (this)->p_head;	\
    else \
      *(p_it) = (*(p_it))->p_next; \
  } while (0); \
  __rv; \
})

/** Retrieve and store in p_value the element referenced to by the iterator p_it.
 **
 ** @return
 ** OML_E_NOT_FOUND if the iterator is past the last element in the list.
 **
 ** @see oml_list_set()
 **/
#define oml_list_get(this, p_it, p_value) ({		\
      oml_rv __rv = OML_OK;				\
      do {						\
	if (! oml_list_has_value((this), (p_it))) {	\
	  __rv = OML_E_NOT_FOUND;			\
	  break;					\
	}						\
	if (*(p_it) == NULL)				\
	  *(p_value) = (this)->p_head->value;		\
	else						\
	  *(p_value) = (*(p_it))->p_next->value;	\
      } while (0);					\
      __rv;						\
    })

/** Direct (L-assignable) reference to the value referenced by p_it.
 **
 ** @note If the iterator is not valid, behaviour is unspecified.
 **/
#define oml_list_value(this, p_it) ((*(p_it) == NULL ? (this)->p_head : (*(p_it))->p_next)->value)

#define oml_list_set(this, p_it, p_value) ({		\
      oml_rv __rv = OML_OK;				\
      do {						\
	if (! oml_list_has_value((this), (p_it))) {	\
	  __rv = OML_E_NOT_FOUND;			\
	  break;					\
	}						\
	if (*(p_it) == NULL)				\
	  (this)->p_head->value = *(p_value);		\
	else						\
	  (*(p_it))->p_next->value = *(p_value);	\
      } while (0);					\
      __rv;						\
    })

/** Get an iterator positioned past the last element. **/
#define oml_list_end(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *(p_it) = (this)->p_tail; \
  } while (0); \
  __rv; \
})

/** Insert a value at the current iterator position in the list.
 **
 ** The element is inserted before the one currently referenced by the iterator,
 ** so that, after the insertion, the element returned by oml_list_get() is
 ** the new inserted value.
 **/
#define oml_list_insert(this, p_it, _value) ({ \
  oml_rv __rv = OML_OK; \
  typeof((this)->p_head->value) __value = (_value); \
  do { \
    typeof((this)->p_head) __p_node = oml_malloc(sizeof(typeof(*(this)->p_head))); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->value = __value; \
    if (*(p_it) == NULL) { \
      __p_node->p_next = (this)->p_head; \
      (this)->p_head = __p_node; \
    } else { \
      __p_node->p_next = (*(p_it))->p_next; \
      (*(p_it))->p_next = __p_node; \
    } \
    if (__p_node->p_next == NULL) \
      (this)->p_tail = __p_node; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

/** Removes the value currently referenced by the iterator */
#define oml_list_remove(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof((this)->p_head) __p_node; \
    if (oml_list_empty(this)) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if (! oml_list_has_value((this), (p_it))) { \
      __rv = OML_E_INCONSISTENT_STATE; \
      break; \
    } \
    if (*(p_it) == NULL) { \
      __p_node = (this)->p_head; \
      (this)->p_head = __p_node->p_next; \
      if ((this)->p_tail == __p_node) \
        (this)->p_tail = NULL; \
    } else { \
      __p_node = (*(p_it))->p_next; \
      (*(p_it))->p_next = __p_node->p_next; \
      if ((this)->p_tail == __p_node) \
        (this)->p_tail = *(p_it); \
    } \
    (this)->num_elems--; \
    oml_free(__p_node); \
  } while (0); \
  __rv; \
})

/** Check if the two iterators refer to the same position **/
#define oml_list_iter_eq(this, p_it1, p_it2) \
  ((*(p_it1)) == (*(p_it2)))

/** Let the supplied iterator iterate over the entire collection **/
#define oml_list_foreach(this, p_it)	\
  for (oml_list_begin(this, p_it); oml_list_has_value(this, p_it); oml_list_next(this, p_it))

#include "oml_list_find.h"

#define oml_list_del(this, _value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof((this)->p_head) it; \
    __rv = oml_list_find((this), _value, &it); \
    if (__rv != OML_OK) \
      break; \
    __rv = oml_list_remove((this), &it); \
  } while (0); \
  __rv; \
})

#endif /* __OML_LIST_H__ */
