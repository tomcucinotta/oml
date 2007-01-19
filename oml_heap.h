#ifndef OML_HEAP_H_
#define OML_HEAP_H_

/**
 * @file
 * 
 * @brief Generic array-based heap implementation
 */

#include "oml_debug.h"

#define oml_define_heap(key_type, value_type) \
  typedef struct oml_heap_node_##key_type ##_##value_type ##_t { \
    key_type key; \
    value_type value; \
  } oml_heap_node_##key_type ##_##value_type ##_t; \
  typedef struct oml_heap_##key_type ##_##value_type ##_t { \
    oml_heap_node_t(key_type, value_type) *elems; \
    int num_elems; \
    int max_num_elems; \
  } oml_heap_##key_type ##_##value_type ##_t

#define oml_heap_node_t(key_type, value_type) \
  struct oml_heap_node_##key_type ##_##value_type ##_t

#define oml_heap_t(key_type, value_type) \
  oml_heap_##key_type ##_##value_type ##_t

#define oml_heap_init(this, N) ({ \
  int __rv = OML_OK; \
  do { \
    (this)->elems = oml_malloc(sizeof((this)->elems[0]) << N); \
    if ((this)->elems == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    (this)->max_num_elems = 1 << N; \
    (this)->num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_heap_add(this, _key, _value) ({ \
  int __rv = OML_OK; \
  int pos = (this)->num_elems + 1; \
  int pos_fath; \
  do { \
    if (pos == (this)->max_num_elems + 1) { \
      __rv = OML_E_FULL; \
      break; \
    } \
    pos_fath = pos / 2; \
    while ((pos != 1) && ((_key) < (this)->elems[pos_fath - 1].key)) { \
      (this)->elems[pos - 1] = (this)->elems[pos_fath - 1]; \
      pos = pos_fath; \
      pos_fath = pos / 2; \
    } \
    (this)->elems[pos - 1].key = (_key); \
    (this)->elems[pos - 1].value = (_value); \
    (this)->num_elems++; \
  } while (0); \
  __rv; \
})

#define oml_heap_get_min(this, p_key, p_value) ({ \
  int __rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    *(p_key) = (this)->elems[0].key; \
    *(p_value) = (this)->elems[0].value; \
  } while (0); \
  __rv; \
})

#define oml_heap_del_min(this) ({ \
  int __rv = OML_OK; \
  int pos = 1; \
  int pos_child; \
  int _key; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    _key = (this)->elems[(this)->num_elems - 1].key; \
    while ((pos < (this)->num_elems) && (_key > (this)->elems[pos - 1].key)) { \
      pos_child = pos * 2; \
      if (_key < (this)->elems[pos_child - 1].key) \
        pos_child++; \
      (this)->elems[pos - 1] = (this)->elems[pos_child - 1]; \
      pos = pos_child; \
    } \
    (this)->elems[pos - 1] = (this)->elems[(this)->num_elems - 1]; \
    (this)->num_elems--; \
  } while (0); \
  __rv; \
})

#define oml_heap_size(this) (this)->num_elems

#endif /*OML_HEAP_H_*/
