#ifndef OML_DLTREE_H_
#define OML_DLTREE_H_

/**
 * @file
 * 
 * @brief Generic double linked tree
 */

#include "oml_debug.h"

#define oml_dltree_instantiate(value_type) \
  typedef struct oml_dltree_node_##value_type ##_t { \
    value_type value; \
    struct oml_dltree_node_##value_type ##_t *p_father, *p_left_child, *p_right_child; \
  } oml_dltree_node_##value_type ##_t; \
  typedef oml_dltree_node_##value_type ##_t *oml_dltree_iterator_##value_type ##_t; \
  typedef struct oml_dltree_##key_type ##_##value_type ##_t { \
    oml_heap_node_t(key_type, value_type) *p_root; \
    int num_elems; \
  } oml_dltree_##key_type ##_##value_type ##_t

#define oml_dltree_node_t(value_type) \
  oml_dltree_node_##value_type ##_t

#define oml_dltree_iterator_t(value_type) \
  oml_dltree_iterator_##value_type ##_t

#define oml_dltree_t(key_type, value_type) \
  oml_dltree_##key_type ##_##value_type ##_t

#define oml_dltree_init(this) ({ \
  int __rv = OML_OK; \
  do { \
    (this)->p_root = NULL; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Inserts _value in place of *it, pushing *it at the left child of _value */
#define oml_dltree_add_left(this, it, _value) ({ \
  int __rv = OML_OK; \
  do { \
    typeof((this)->p_root) *__p_node = oml_malloc(sizeof(*typeof((this)->p_root))); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    if ((it) == NULL || (it) == (this)->p_root) { /* Replace Root */ \
      __p_node->p_left_child = (this)->p_root; \
      __p_node->p_right_child = NULL; \
      __p_node->p_father = NULL; \
      if ((this)->p_root != NULL) \
        (this)->p_root->p_father = __p_node; \
      (this)->p_root = __p_node; \
      (this)->num_elems++; \
      break; \
    } \
    /* Replace non-root */ \
    __p_node->p_left_child = (it); \
    __p_node->p_right_child = NULL; \
    __p_node->p_father = (it)->p_father; \
    (it)->p_father = __p_node; \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

#define oml_heap_size(this) (this)->num_elems

#endif /*OML_HEAP_H_*/
