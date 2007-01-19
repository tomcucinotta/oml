#ifndef OML_TREEHEAP_H_
#define OML_TREEHEAP_H_

/**
 * @file
 * 
 * @brief Generic tree-based heap implementation
 */

#include "oml_debug.h"

#define oml_define_theap(key_type, value_type) \
  typedef struct oml_theap_node_##key_type ##_##value_type ##_t { \
    key_type key; \
    value_type value; \
    struct oml_theap_node_##key_type ##_##value_type ##_t *p_fath, *p_left_child, *p_right_child; \
    struct oml_theap_node_##key_type ##_##value_type ##_t *p_prev, *p_next; /* Circular list of nodes of the same level */ \
  } oml_theap_node_##key_type ##_##value_type ##_t; \
  typedef struct { \
    oml_theap_node_##key_type ##_##value_type ##_t *p_first_leaf; \
    oml_theap_node_##key_type ##_##value_type ##_t *p_node; \
  } oml_theap_iterator_##key_type ##_##value_type ##_t; \
  typedef struct oml_theap_##key_type ##_##value_type ##_t { \
    oml_theap_node_t(key_type, value_type) *p_root; \
    oml_theap_node_t(key_type, value_type) *p_first_leaf; \
    oml_theap_node_t(key_type, value_type) *p_last_leaf; \
    /* Fictious element used for typeof() purposes */ \
    oml_theap_iterator_t(key_type, value_type) *p_it; \
    int num_elems; \
  } oml_theap_##key_type ##_##value_type ##_t

#define oml_theap_node_t(key_type, value_type) \
  oml_theap_node_##key_type ##_##value_type ##_t

#define oml_theap_iterator_t(key_type, value_type) \
  oml_theap_iterator_##key_type ##_##value_type ##_t

#define oml_theap_t(key_type, value_type) \
  oml_theap_##key_type ##_##value_type ##_t

#define oml_theap_iterator_begin(this, p_it) \
  do { \
    (p_it)->p_first_leaf = (this)->p_root; \
    (p_it)->p_node = (this)->p_root; \
  } while (0)

#define oml_theap_iterator_eoi(p_it) \
  ((p_it)->p_node == NULL)

#define oml_theap_iterator_next(p_it) \
  do { \
    (p_it)->p_node = (p_it)->p_node->p_next; \
    if ((p_it)->p_node == (p_it)->p_first_leaf) { \
      (p_it)->p_first_leaf = (p_it)->p_first_leaf->p_left_child; \
      (p_it)->p_node = (p_it)->p_first_leaf; \
    } \
  } while (0)

#define oml_theap_iterator_get_key(p_it) \
  (p_it)->p_node->key

#define oml_theap_iterator_get_value(p_it) \
  (p_it)->p_node->value

#define oml_theap_dump_node(p_node) \
  do { \
    oml_log_debug("(ptr, k, v, f, l, r, p, n) = (%p, %d, %d, %p, %p, %p, %p, %p)", \
      p_node, p_node->key, p_node->value, p_node->p_fath, p_node->p_left_child, p_node->p_right_child, \
      p_node->p_prev, p_node->p_next \
    ); \
  } while (0)

#define oml_theap_dump(this) ({ \
  int __rv = OML_OK; \
  do { \
    typeof((this)->p_root) __p_first_leaf = (this)->p_root; \
    typeof((this)->p_root) __p_dnode = (this)->p_root; \
    oml_log_debug("HEAP DUMP: (#, r, fl, ll) = (%d, %p, %p, %p)", (this)->num_elems, (this)->p_root, (this)->p_first_leaf, (this)->p_last_leaf); \
    while (__p_dnode != NULL) { \
      do { \
        oml_theap_dump_node(__p_dnode); \
        __p_dnode = __p_dnode->p_next; \
      } while (__p_dnode != __p_first_leaf); \
      oml_log_debug("-"); \
      __p_first_leaf = __p_first_leaf->p_left_child; \
      __p_dnode = __p_first_leaf; \
    } \
  } while (0); \
  __rv; \
})

#define oml_theap_init(this) ({ \
  int __rv = OML_OK; \
  do { \
    (this)->p_root = NULL; \
    (this)->p_first_leaf = NULL; \
    (this)->p_last_leaf = NULL; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_swap_ptr(p_node1, p_node2, field) \
  do { \
    typeof((p_node1)->field) __tmp1 = (p_node2)->field; \
    typeof((p_node2)->field) __tmp2 = (p_node1)->field; \
    if (__tmp1 == p_node1) __tmp1 = (p_node2); \
    else if (__tmp1 == p_node2) __tmp1 = (p_node1); \
    if (__tmp2 == p_node1) __tmp2 = (p_node2); \
    else if (__tmp2 == p_node2) __tmp2 = (p_node1); \
    (p_node1)->field = __tmp1; \
    (p_node2)->field = __tmp2; \
  } while (0)

#define oml_theap_chg_ptr(this, field, p_node1, p_node2) \
  do { \
    if ((this)->field == (p_node1)) \
      (this)->field = (p_node2); \
    else if ((this)->field == (p_node2)) \
      (this)->field = (p_node1); \
  } while (0)

#define oml_theap_swap_link(p_node1, p_node2, field1, field2) \
  do { \
    typeof(p_node1) __sl_p1 = (p_node1); \
    typeof(p_node2) __sl_p2 = (p_node2); \
    if (__sl_p1->field1 != __sl_p1 && __sl_p1->field1 != __sl_p2) \
      __sl_p1->field1->field2 = __sl_p2; \
    if (__sl_p2->field1 != __sl_p2 && __sl_p2->field1 != __sl_p1) \
      __sl_p2->field1->field2 = __sl_p1; \
  } while (0)

#define oml_theap_swap_link_safe(p_node1, p_node2, field1, field2) \
  do { \
    typeof(p_node1) __sls_p1 = (p_node1); \
    typeof(p_node2) __sls_p2 = (p_node2); \
    if (__sls_p1->field1 != NULL && __sls_p1->field1->field2 != NULL && __sls_p1->field1 != __sls_p1 && __sls_p1->field1 != __sls_p2) \
      __sls_p1->field1->field2 = __sls_p2; \
    if (__sls_p2->field1 != NULL && __sls_p2->field1->field2 != NULL && __sls_p2->field1 != __sls_p2 && __sls_p2->field1 != __sls_p1) \
      __sls_p2->field1->field2 = __sls_p1; \
  } while (0)

#define oml_theap_swap_fath(p_node1, p_node2) \
  do { \
    typeof(p_node1) __sf_p1 = (p_node1); \
    typeof(p_node2) __sf_p2 = (p_node2); \
    if (__sf_p1->p_fath != NULL && __sf_p1->p_fath != __sf_p2 && __sf_p1->p_fath->p_left_child == __sf_p1) \
      __sf_p1->p_fath->p_left_child = __sf_p2; \
    else if (__sf_p1->p_fath != NULL && __sf_p1->p_fath != __sf_p2 && __sf_p1->p_fath->p_right_child == __sf_p1) \
      __sf_p1->p_fath->p_right_child = __sf_p2; \
    if (__sf_p2->p_fath != NULL && __sf_p2->p_fath != __sf_p1 && __sf_p2->p_fath->p_left_child == __sf_p2) \
      __sf_p2->p_fath->p_left_child = __sf_p1; \
    else if (__sf_p2->p_fath != NULL && __sf_p2->p_fath != __sf_p1 && __sf_p2->p_fath->p_right_child == __sf_p2) \
      __sf_p2->p_fath->p_right_child = __sf_p1; \
  } while (0)

oml_define_theap(int, int);

void oml_theap_swap_node(struct oml_theap_t(int, int) *this, struct oml_theap_node_t(int, int) *p_node1, struct oml_theap_node_t(int, int) *p_node2) { \
  do { \
    typeof(p_node1) __sn_p1 = (p_node1); \
    typeof(p_node2) __sn_p2 = (p_node2); \
    oml_log_debug("Swapping nodes: %p (%d) and %p (%d)", p_node1, p_node1->key, p_node2, p_node2->key); \
/*    oml_theap_dump(this); \
*/    if (__sn_p1 == __sn_p2) \
      break; \
    oml_theap_swap_link(__sn_p1, __sn_p2, p_prev, p_next); \
    oml_theap_swap_link(__sn_p1, __sn_p2, p_next, p_prev); \
    oml_theap_swap_link_safe(__sn_p1, __sn_p2, p_left_child, p_fath); \
    oml_theap_swap_link_safe(__sn_p1, __sn_p2, p_right_child, p_fath); \
    oml_theap_swap_fath(__sn_p1, __sn_p2); \
    oml_swap_ptr(__sn_p1, __sn_p2, p_fath); \
    oml_swap_ptr(__sn_p1, __sn_p2, p_left_child); \
    oml_swap_ptr(__sn_p1, __sn_p2, p_right_child); \
    oml_swap_ptr(__sn_p1, __sn_p2, p_next); \
    oml_swap_ptr(__sn_p1, __sn_p2, p_prev); \
    oml_theap_chg_ptr(this, p_root, __sn_p1, __sn_p2); \
    oml_theap_chg_ptr(this, p_first_leaf, __sn_p1, __sn_p2); \
    oml_theap_chg_ptr(this, p_last_leaf, __sn_p1, __sn_p2); \
/*    oml_theap_dump(this); \
*/  } while (0); \
}

#define oml_theap_add(this, _key, _value) ({ \
  int __rv = OML_OK; \
  do { \
    typeof((this)->p_root) __p_node = oml_malloc(sizeof(typeof(*(this)->p_root))); \
    if (__p_node == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    __p_node->key = (_key); \
    __p_node->value = (_value); \
    __p_node->p_left_child = NULL; \
    __p_node->p_right_child = NULL; \
    if ((this)->p_root == NULL) { \
      /* Empty heap */ \
      oml_log_debug("Empty heap"); \
      __p_node->p_fath = NULL; \
      __p_node->p_prev = __p_node->p_next = __p_node; \
      (this)->p_root = __p_node; \
      (this)->p_first_leaf = __p_node; \
      (this)->p_last_leaf = __p_node; \
      ++((this)->num_elems); \
      break; \
    } else if ( \
         ((this)->p_last_leaf->p_fath == NULL) \
         || ( ((this)->p_last_leaf->p_fath->p_right_child != NULL) \
              && ((this)->p_last_leaf->p_fath->p_next->p_left_child != NULL)) \
    ) { \
      /* Last leaf is the rightmost one, thus new node is a first leaf and a left child */ \
      oml_log_debug("New node starts new heap level"); \
      __p_node->p_fath = (this)->p_first_leaf; \
      __p_node->p_prev = __p_node->p_next = __p_node; \
      oml_assert((this)->p_first_leaf->p_left_child == NULL); \
      oml_assert((this)->p_first_leaf->p_right_child == NULL); \
      (this)->p_first_leaf->p_left_child = __p_node; \
      (this)->p_first_leaf = __p_node; \
      (this)->p_last_leaf = __p_node; \
    } else { \
      /* Last leaf is not the rightmost one */ \
      oml_log_debug("New node continues last heap (uncomplete) level"); \
      if ((this)->p_last_leaf->p_fath->p_right_child == NULL) { \
        /* New node is direct brother of last leaf, thus new node is a right child */ \
        oml_log_debug("New node is a right-child"); \
        oml_assert((this)->p_last_leaf->p_fath->p_left_child == (this)->p_last_leaf); \
        oml_log_debug("Assert passed"); \
        __p_node->p_fath = (this)->p_last_leaf->p_fath; \
        __p_node->p_fath->p_right_child = __p_node; \
        (this)->p_last_leaf->p_next = __p_node; \
        __p_node->p_prev = (this)->p_last_leaf; \
        (this)->p_first_leaf->p_prev = __p_node; \
        __p_node->p_next = (this)->p_first_leaf; \
      } else { \
        /* New node is cousin (any degree is possible) of last leaf, thus it is a left child */ \
        oml_log_debug("New node is a left-child"); \
        __p_node->p_fath = (this)->p_last_leaf->p_fath->p_next; \
        oml_assert(__p_node->p_fath->p_left_child == NULL); \
        oml_assert(__p_node->p_fath->p_right_child == NULL); \
        oml_log_debug("Assert passed"); \
        __p_node->p_fath->p_left_child = __p_node; \
        (this)->p_last_leaf->p_next = __p_node; \
        __p_node->p_prev = (this)->p_last_leaf; \
        (this)->p_first_leaf->p_prev = __p_node; \
        __p_node->p_next = (this)->p_first_leaf; \
      } \
      (this)->p_last_leaf = __p_node; \
    } \
    while ((__p_node != (this)->p_root) && (__p_node->key < __p_node->p_fath->key)) { \
      typeof((this)->p_root) __p_fath = __p_node->p_fath; \
      oml_theap_swap_node((this), __p_node, __p_fath); \
    } \
    ++((this)->num_elems); \
  } while (0); \
  __rv; \
})

#define oml_theap_del_last(this) ({ \
  int __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } else if ((this)->num_elems == 1) { \
      oml_free((this)->p_root); \
      __rv = oml_theap_init(this); \
      break; \
    } \
    if ((this)->p_last_leaf == (this)->p_first_leaf) { \
      /* Deleting unique elem of level */ \
      oml_log_debug("Deleting unique elem of level"); \
      (this)->p_last_leaf->p_fath->p_left_child = NULL; \
      assert((this)->p_last_leaf->p_fath->p_right_child == NULL); \
      (this)->p_first_leaf = (this)->p_last_leaf->p_fath; \
      oml_free((this)->p_last_leaf); \
      (this)->p_last_leaf = (this)->p_first_leaf->p_prev; \
      oml_log_debug("Done"); \
    } else { \
      /* Deleting non-first elem of level */ \
      oml_log_debug("Deleting non-first elem of level"); \
      if ((this)->p_last_leaf->p_fath->p_right_child == (this)->p_last_leaf) \
        (this)->p_last_leaf->p_fath->p_right_child = NULL; \
      else \
        (this)->p_last_leaf->p_fath->p_left_child = NULL; \
      (this)->p_first_leaf->p_prev = (this)->p_last_leaf->p_prev; \
      oml_free((this)->p_last_leaf); \
      (this)->p_last_leaf = (this)->p_first_leaf->p_prev; \
      (this)->p_last_leaf->p_next = (this)->p_first_leaf; \
    } \
    --((this)->num_elems); \
  } while (0); \
  __rv; \
})

#define oml_theap_del_min(this) ({ \
  int __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } else { \
      typeof((this)->p_root) __p_node; \
      oml_theap_swap_node((this), (this)->p_root, (this)->p_last_leaf); \
      oml_theap_dump(this); \
      __rv = oml_theap_del_last(this); \
      oml_theap_dump(this); \
      __p_node = (this)->p_root; \
      while (__p_node != NULL && __p_node->p_left_child != NULL) { \
        oml_theap_dump(this); \
        if (__p_node->p_left_child->key < __p_node->key) { \
          typeof((this)->p_root) __p_child = __p_node->p_left_child; \
          oml_theap_swap_node((this), __p_node, __p_child); \
        } else if (__p_node->p_right_child != NULL && __p_node->p_right_child->key < __p_node->key) { \
          typeof((this)->p_root) __p_child = __p_node->p_right_child; \
          oml_theap_swap_node((this), __p_node, __p_child); \
        } else \
          __p_node = NULL; \
      } \
    } \
  } while (0); \
  __rv; \
})

#define oml_theap_get_min(this, p_key, p_value) ({ \
  int __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    *(p_key) = (this)->p_root->key; \
    *(p_value) = (this)->p_root->value; \
  } while (0); \
  __rv; \
})

#define oml_theap_size(this) (this)->num_elems

#define oml_theap_cleanup(this) \
  do { \
    typeof(*(this)->p_it) it; \
    oml_theap_iterator_begin(this, &it); \
    while (! oml_theap_iterator_eoi(&it)) { \
      typeof((this)->p_root) __p_node = it.p_node; \
      oml_theap_iterator_next(&it); \
      oml_free(__p_node); \
    } \
  } while (0)

#endif /*OML_HEAP_H_*/
