#ifndef OML_DLTREE_H_
#define OML_DLTREE_H_

/**
 * @file
 * 
 * @brief Generic double linked tree
 */

#include "oml_debug.h"
#include "oml_malloc.h"

#define oml_define_dltree(value_type) \
  typedef struct oml_dltree_node_##value_type ##_t { \
    struct oml_dltree_node_##value_type ##_t *p_parent, *p_left_child, *p_right_child; \
    value_type value; \
  } oml_dltree_node_##value_type ##_t; \
  typedef oml_dltree_node_##value_type ##_t *oml_dltree_iterator_##value_type ##_t; \
  typedef struct oml_dltree_##value_type ##_t { \
    oml_dltree_node_##value_type ##_t *p_root; \
    int num_elems; \
  } oml_dltree_##value_type ##_t

#define oml_dltree_node(value_type) \
  oml_dltree_node_##value_type ##_t

#define oml_dltree_iterator(value_type) \
  oml_dltree_iterator_##value_type ##_t

#define oml_dltree_value_type(this) typeof((this)->p_root->value)

#define oml_dltree(value_type) \
  oml_dltree_##value_type ##_t

#define oml_dltree_init(this) ({ \
  typeof(this) __this = (this); \
  oml_rv __rv = OML_OK; \
  do { \
    (__this)->p_root = NULL; \
    (__this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

struct oml_dltree_node {
  struct oml_dltree_node *p_parent, *p_left_child, *p_right_child;
};

static void __dltree_cleanup(struct oml_dltree_node *p) {
  if (p != NULL) {
    __dltree_cleanup(p->p_left_child);
    __dltree_cleanup(p->p_right_child);
    oml_free(p);
  }
}

#define oml_dltree_clear(this) ({ \
  oml_rv __rv = OML_OK; \
  typeof(this) __this = (this); \
  __dltree_cleanup((struct oml_dltree_node *) __this->p_root); \
  __this->p_root = NULL; \
  __this->num_elems = 0; \
  __rv; \
})

#define oml_dltree_cleanup(this) \
  oml_dltree_clear(this)

/** Inserts _value in place of *it, pushing *it as the left child of _value */
#define oml_dltree_push_left(this, it, _value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof((this)->p_root) __p_node = oml_malloc(sizeof(*(this)->p_root)); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->value = (_value); \
    __p_node->p_left_child = (it); \
    __p_node->p_right_child = NULL; \
    __p_node->p_parent = (it) == NULL ? NULL : (it)->p_parent; \
    if ((it) != NULL) { \
      if ((it)->p_parent != NULL) { \
        if ((it)->p_parent->p_left_child == it)  \
          (it)->p_parent->p_left_child = __p_node;      \
        else \
          (it)->p_parent->p_right_child = __p_node;     \
      } \
      (it)->p_parent = __p_node; \
    } \
    if ((it) == (this)->p_root) \
      (this)->p_root = __p_node; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

/** Inserts _value in place of *it, pushing *it as the right child of _value */
#define oml_dltree_push_right(this, it, _value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof((this)->p_root) __p_node = oml_malloc(sizeof(*(this)->p_root)); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->value = (_value); \
    __p_node->p_left_child = NULL; \
    __p_node->p_right_child = (it); \
    __p_node->p_parent = (it) == NULL ? NULL : (it)->p_parent; \
    if ((it) != NULL) { \
      if ((it)->p_parent != NULL) { \
        if ((it)->p_parent->p_left_child == it)  \
          (it)->p_parent->p_left_child = __p_node;      \
        else \
          (it)->p_parent->p_right_child = __p_node;     \
      } \
      (it)->p_parent = __p_node; \
    } \
    if ((it) == (this)->p_root) \
      (this)->p_root = __p_node; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

#define oml_dltree_size(this) ((this)->num_elems)

#define oml_dltree_empty(this) ((this)->num_elems == 0)

/* FORWARD ITERATOR */

#define oml_dltree_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *p_it = (this)->p_root; \
  } while (0); \
  __rv; \
})

#define oml_dltree_end(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    *p_it = NULL; \
  } while (0); \
  __rv; \
})

/** Retrieve the next element of a forward iteration, if any. **/
#define oml_dltree_value(this, it) ((it)->value)

#define oml_dltree_get(this, it, p_value) ({			\
      oml_rv __rv = OML_OK;					\
      do {							\
	if (oml_dltree_has_value((this), (it)))                 \
	  *(p_value) = oml_dltree_value((this), (it));          \
	else							\
	  __rv = OML_E_NOT_FOUND;				\
      } while (0);						\
      __rv;							\
    })

/** Retrieves the value from the root of the tree, if any. */
#define oml_dltree_front(this, p_value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if (oml_dltree_empty(this)) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if ((p_value) != NULL)			  \
      *(p_value) = (this)->p_root->value; \
  } while (0); \
  __rv; \
})

/** Check if we may call oml_dltree_get() or oml_dltree_value() **/
#define oml_dltree_has_value(this, it) \
  ((it) != NULL)

/** Check if iterator is at end (past the last element) **/
#define oml_dltree_at_end(this, p_it) \
  (*(p_it) == NULL)

/** Check if iterator is at begin (on the first element or no-valued) **/
#define oml_dltree_at_begin(this, p_it) \
  ((*p_it)->pos == NULL)

/** Move the iterator one element forward. */
#define oml_dltree_next(this, p_it) ({ \
  typeof(p_it) __p_it = (p_it);    \
  oml_rv __rv = OML_OK; \
  do { \
    if (! oml_dltree_has_value((this), *__p_it)) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    if ((*__p_it)->p_left_child) \
      *__p_it = (*__p_it)->p_left_child; \
    else if ((*__p_it)->p_right_child) \
      *__p_it = (*__p_it)->p_right_child; \
    else { \
      __rv = OML_E_NOT_FOUND; \
      do { \
        if ((*__p_it)->p_parent && *__p_it == (*__p_it)->p_parent->p_left_child \
            && (*__p_it)->p_parent->p_right_child) { \
          *__p_it = (*__p_it)->p_right_child; \
          __rv = OML_OK; \
          break; \
        } \
        *__p_it = (*__p_it)->p_parent; \
      } while (*__p_it != NULL); \
    } \
  } while (0); \
  __rv; \
})

#endif /*OML_HEAP_H_*/
