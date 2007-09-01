#ifndef __OML_HEAP_H__
#define __OML_HEAP_H__

/**
 * @file
 * 
 * @brief Generic array-based extraction-efficient min-heap implementation.
 * 
 * @note If you need to compare keys with anything different from the operator
 * "<", then you should use the key_heap_add_lt and key_heap_del_lt versions
 * of the insert and remove operations, which support the name of the comparison
 * function or macro as further argument (look at oml_default_lt in oml_common.h
 * for details).
 */

#include "oml_debug.h"
#include "oml_malloc.h"
#include "oml_common.h"

/** Define heap related types: use this macro only once per compilation unit */
#define oml_define_heap(key_type, value_type) \
  typedef struct oml_heap_iterator_##key_type ##_##value_type { \
    int pos; \
  } oml_heap_iterator_##key_type ##_##value_type; \
  typedef struct oml_heap_node_##key_type ##_##value_type { \
    key_type key; \
    value_type value; \
  } oml_heap_node_##key_type ##_##value_type; \
  typedef struct oml_heap_##key_type ##_##value_type { \
    oml_heap_node(key_type, value_type) *elems; \
    int num_elems; \
    int max_num_elems; \
  } oml_heap_##key_type ##_##value_type

/** Template-like type for a heap node (users should not use this) */
#define oml_heap_node(key_type, value_type) \
  struct oml_heap_node_##key_type ##_##value_type

/** Template-like type for a heap container */
#define oml_heap(key_type, value_type) \
  oml_heap_##key_type ##_##value_type

/** Template-like type for a heap iterator */
#define oml_heap_iterator(key_type, value_type) \
  oml_heap_iterator_##key_type ##_##value_type

/** Initialize an empty heap with (initial) max storage capacity of N
 * 
 * @todo
 *   Investigate possibility of use of realloc() or similar within the kernel
 */
#define oml_heap_init(this, N) ({ \
  oml_rv __rv = OML_OK; \
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

#define oml_heap_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_free((this)->elems); \
    (this)->num_elems = (this)->max_num_elems = 0; \
  } while (0); \
  __rv; \
})

/** Move a node contents from src_pos to dst_pos, overwriting destination contents */
#define oml_heap_nodecpy(this, dst_pos, src_pos) \
  do { \
    (this)->elems[(dst_pos) - 1] = (this)->elems[(src_pos) - 1]; \
  } while (0)

/** Push the specified key down the heap, as if it started
 *  from position (1-based) param_pos, moving up all the necessary items.
 *  Returns the correct position, in which the element has not
 *  been copied yet.
 */
#define oml_heap_push_down(this, param_pos, param_key, key_lt) ({ \
  int _pd_pos = (param_pos); \
  do { \
    int _pd_pos_child; \
    typeof((this)->elems[0].key) _pd_key = (param_key); \
/*    printf("pos: %d\n", _pd_pos);  fflush(stdout); \
*/      _pd_pos_child = _pd_pos * 2; \
    while ( \
        ((_pd_pos_child <= (this)->num_elems) && key_lt((this)->elems[_pd_pos_child - 1].key, _pd_key)) \
        || ((_pd_pos_child + 1 <= (this)->num_elems) && key_lt((this)->elems[_pd_pos_child].key, _pd_key)) \
      ) { \
        if ((_pd_pos_child + 1 <= (this)->num_elems) && key_lt((this)->elems[_pd_pos_child].key, (this)->elems[_pd_pos_child - 1].key)) \
          _pd_pos_child++; \
        oml_heap_nodecpy((this), _pd_pos, _pd_pos_child); \
        _pd_pos = _pd_pos_child; \
        _pd_pos_child = _pd_pos * 2; \
/*      printf("pos: %d\n", _pd_pos); \
*/    } \
  } while (0); \
  _pd_pos; \
})

/** Push the specified key up the heap, as if it started
 *  from position (1-based) param_pos, moving down all the necessary items.
 *  Returns the correct position, in which the element has not
 *  been copied yet.
 */
#define oml_heap_push_up(this, param_pos, param_key, key_lt) ({ \
  int _pu_pos = (param_pos); \
  do { \
    int _pu_pos_fath = _pu_pos / 2; \
/*    printf("xpos: %d\n", _pu_pos);  fflush(stdout); \
*/    while ((_pu_pos != 1) && key_lt((param_key), (this)->elems[_pu_pos_fath - 1].key)) { \
      oml_heap_nodecpy((this), _pu_pos, _pu_pos_fath); \
      _pu_pos = _pu_pos_fath; \
      _pu_pos_fath = _pu_pos / 2; \
/*      printf("pos: %d\n", _pu_pos);  fflush(stdout); \
*/    } \
  } while (0); \
  _pu_pos; \
})

/**
 * Add the pair (_key, _value) to the heap, and store the corresponding
 * removal iterator in *_p_it.
 */
#define oml_heap_add_lt(this, _key, _value, _op_key_lt) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    int _a_pos = (this)->num_elems + 1; \
    if (_a_pos == (this)->max_num_elems + 1) { \
      __rv = OML_E_FULL; \
      break; \
    } \
    (this)->num_elems++; \
    _a_pos = oml_heap_push_up((this), _a_pos, (_key), _op_key_lt); \
    (this)->elems[_a_pos - 1].key = (_key); \
    (this)->elems[_a_pos - 1].value = (_value); \
  } while (0); \
  __rv; \
})

/**
 * Add the pair (_key, _value) to the heap, and store the corresponding
 * removal iterator in *_p_it.
 */
#define oml_heap_add(this, _key, _value) oml_heap_add_lt((this), (_key), (_value), oml_default_lt)

/** Return (without removing) the heap root pair, corresponding to the minimum key value */
#define oml_heap_get_min(this, p_key, p_value) ({ \
  oml_rv __gm_rv = OML_OK; \
  do { \
    if ((this)->num_elems == 0) { \
      __gm_rv = OML_E_EMPTY; \
      break; \
    } \
    *(p_key) = (this)->elems[0].key; \
    *(p_value) = (this)->elems[0].value; \
  } while (0); \
  __gm_rv; \
})

/**
 * Remove the top (root) of the heap, alias the minimum-key element.
 * 
 * @note
 *   The corresponding iterator, if any, is not deallocated, but it is invalidated.
 */
#define oml_heap_del_min_lt(this, op_key_lt) ({ \
  oml_rv __dm_rv = OML_OK; \
  int _dm_pos = 1; \
  int _dm_key; \
  do { \
    if ((this)->num_elems == 0) { \
      __dm_rv = OML_E_EMPTY; \
      break; \
    } \
    _dm_key = (this)->elems[(this)->num_elems - 1].key; \
    (this)->num_elems--; \
/*    printf("Pushing down pos: %d\n", _dm_pos); \
*/    _dm_pos = oml_heap_push_down((this), _dm_pos, _dm_key, op_key_lt); \
/*    printf("Copying into pos: %d\n", _dm_pos); \
*/    /* This copies on purpose from a position == num_elems + 1, 'cause we know it is legal */ \
    oml_heap_nodecpy((this), _dm_pos, (this)->num_elems + 1); \
  } while (0); \
  __dm_rv; \
})

/**
 * Remove the top (root) of the heap, alias the minimum-key element.
 * 
 * @note
 *   The corresponding iterator, if any, is not deallocated, but it is invalidated.
 */
#define oml_heap_del_min(this) oml_heap_del_min_lt((this), oml_default_lt)

/**
 * Remove an arbitrary element of the heap, through the iterator stored during a
 * oml_heap_add() operation.
 * 
 * @note
 *   The corresponding iterator is not deallocated, but it is invalidated.
 */
#define oml_heap_del_lt(this, _d_pos, op_key_lt) ({ \
  oml_rv __rv = OML_OK; \
  int _d_pos2; \
  typeof((this)->elems[0].key) _d_key; \
  do { \
    if ((this)->num_elems == 0) { \
      __rv = OML_E_EMPTY; \
      break; \
    } \
    if (_d_pos == 0) { /* Invalid iterator detection */ \
      __rv = OML_E_INVALID_PARAM; \
      break; \
    } \
    _d_key = (this)->elems[(this)->num_elems - 1].key; \
    (this)->num_elems--; \
    if ((this)->num_elems == 0 || (this)->num_elems + 1 == _d_pos) \
      break; \
    _d_pos2 = oml_heap_push_down((this), _d_pos, _d_key, op_key_lt); \
    if (_d_pos2 == _d_pos) \
      _d_pos2 = oml_heap_push_up((this), _d_pos, _d_key, op_key_lt); \
    /* We use on purpose num_elems + 1, as we know it is legal */ \
    oml_heap_nodecpy((this), _d_pos2, (this)->num_elems + 1); \
  } while (0); \
  __rv; \
})

/** Return the current size of the heap */
#define oml_heap_size(this) ((this)->num_elems)

#define oml_heap_begin(this, p_it) \
  do { \
    (p_it)->pos = 0; \
  } while (0)

#define oml_heap_has_value(this, p_it) \
  ((p_it)->pos < (this)->num_elems)

#define oml_heap_get_next(this, p_it, p_key, p_val) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((p_it)->pos >= (this)->num_elems) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    *(p_key) = (this)->elems[(p_it)->pos].key; \
    *(p_val) = (this)->elems[(p_it)->pos].value; \
  } while (0); \
  __rv; \
})

#define oml_heap_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((p_it)->pos >= (this)->num_elems) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    (p_it)->pos++; \
  } while (0); \
  __rv; \
})

#endif /* __OML_HEAP_H__ */
