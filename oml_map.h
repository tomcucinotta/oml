#ifndef __OML_MAP_H__
#define __OML_MAP_H__

/** @file
 ** 
 ** @brief Generic hashmap.
 ** 
 ** @note
 ** If you need to hash keys with anything different from the oml_default_hash()
 ** function, then
 **/

#include "oml_debug.h"
#include "oml_malloc.h"
#include "oml_common.h"
#include "oml_pair.h"
#include "oml_list.h"
#include "oml_list_find.h"

/** Define map related types: use this macro only once per compilation unit */
#define oml_define_map(key_type, value_type) \
  oml_define_pair(key_type, value_type); \
  typedef oml_pair(key_type, value_type) oml_map_node_##key_type ##_##value_type; \
  oml_define_list(oml_map_node_##key_type ##_##value_type); \
  typedef struct oml_map_iterator_##key_type ##_##value_type { \
    int pos; \
    oml_list_iterator(oml_map_node_##key_type ##_##value_type) it; \
  } oml_map_iterator_##key_type ##_##value_type; \
  typedef struct oml_map_##key_type ##_##value_type { \
    oml_list(oml_map_node_##key_type ##_##value_type) *elems; \
    int num_elems; \
    int max_num_elems; \
    oml_list_iterator(oml_map_node_##key_type ##_##value_type) *p_it; \
  } oml_map_##key_type ##_##value_type

/** Template-like type for a map node (users should not use this) */
#define oml_map_node(key_type, value_type) \
  struct oml_map_node_##key_type ##_##value_type

/** Template-like type for a map container */
#define oml_map(key_type, value_type) \
  oml_map_##key_type ##_##value_type

/** Template-like type for a map iterator */
#define oml_map_iterator(key_type, value_type) \
  oml_map_iterator_##key_type ##_##value_type

/** Initialize an empty map with a hashtable size of 2 ^ N **/
#define oml_map_init(this, N) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    int _pos; \
    (this)->elems = oml_malloc(sizeof((this)->elems[0]) << N); \
    if ((this)->elems == NULL) { \
      __rv = OML_E_NO_MEMORY; \
      break; \
    } \
    (this)->max_num_elems = 1 << N; \
    (this)->num_elems = 0; \
    for (_pos = 0; _pos < (this)->max_num_elems; _pos++) \
      oml_list_init(&(this)->elems[_pos]);		 \
  } while (0); \
  __rv; \
})

#define oml_map_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    int _pos; \
    for (_pos = 0; _pos < (this)->max_num_elems; _pos++) \
      oml_list_cleanup(&(this)->elems[_pos]);		 \
    oml_free((this)->elems); \
    (this)->num_elems = (this)->max_num_elems = 0; \
  } while (0); \
  __rv; \
})

#define oml_map_node_eq(n1, k2, f_key_eq) (f_key_eq(oml_pair_first(&(n1)), (k2)))

/**
 ** Add the pair (_key, _value) to the map
 **
 ** @note Adding a second time the same key to the map results in replacing the previously associated value.
 **/
#define oml_map_add_eq(this, _key, _value, _op_key_eq) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof(_key) __key = (_key); \
    int _a_pos = oml_default_hash(__key) % ((this)->max_num_elems); \
    typeof(*((this)->p_it)) it; \
    __rv = oml_list_find_eq_param(&((this)->elems[_a_pos]), _key, &it, oml_map_node_eq, _op_key_eq); \
    if (__rv == OML_E_NOT_FOUND) { \
      oml_list_value_type(&((this)->elems[_a_pos]))_pair;	\
      oml_pair_init(&_pair, _key, _value); \
      __rv = oml_list_push_front(&((this)->elems[_a_pos]), _pair); \
      if (__rv != OML_OK) \
        break; \
      (this)->num_elems++;                           \
    } else { \
      typeof(&((this)->elems[_a_pos])) p_list = &((this)->elems[_a_pos]); \
      oml_list_value_type( p_list ) __p;				\
      oml_list_get( p_list, &it, &__p );				\
      oml_pair_second( &__p ) = _value;					\
      oml_list_set( p_list, &it, &__p );        \
    } \
  } while (0); \
  __rv; \
})


/**
 * Add the pair (_key, _value) to the map, and store the corresponding
 * removal iterator in *_p_it.
 */
#define oml_map_add(this, _key, _value) oml_map_add_eq((this), (_key), (_value), oml_default_eq)


/**
 ** Write corresponding value of _key in p_value
 **/
#define oml_map_get_eq(this, _key, p_value, _op_key_eq) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    typeof(_key) __key = (_key); \
    int _a_pos = oml_default_hash(__key) % ((this)->max_num_elems); \
    typeof(*((this)->p_it)) it; \
    __rv = oml_list_find_eq_param(&((this)->elems[_a_pos]), _key, &it, oml_map_node_eq, _op_key_eq); \
    if (__rv == OML_E_NOT_FOUND) { \
      break; \
    } else { \
      typeof(&((this)->elems[_a_pos])) p_list = &((this)->elems[_a_pos]); \
      oml_list_value_type( p_list ) __p;				\
      oml_list_get( p_list, &it, &__p );				\
      *(p_value) = oml_pair_second(&__p);       \
    } \
  } while (0); \
  __rv; \
})

#define oml_map_get(this, _key, p_value) oml_map_get_eq((this), (_key), (p_value), oml_default_eq)

/** Return the current size of the map */
#define oml_map_size(this) ((this)->num_elems)

#define oml_map_begin(this, p_it) ({                          \
  oml_rv __rv = OML_OK;                                       \
  do {                                                        \
    if ((this)->num_elems > 0) {                              \
      (p_it)->pos = 0;                                        \
      while (oml_list_size(&(this)->elems[(p_it)->pos]) == 0) \
        (p_it)->pos++;                                        \
      oml_list_const_begin(&(this)->elems[(p_it)->pos], &(p_it)->it); \
    } else {                                                  \
      (p_it)->pos = -1;                                       \
      __rv = OML_E_EMPTY;                                     \
    }                                                         \
  } while (0);                                                \
  __rv;                                                       \
})

#define oml_map_has_value(this, p_it) \
  ((p_it)->pos != -1)

#define oml_map_get_iter(this, p_it, p_key, p_val) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    if ((p_it)->pos == -1) { \
      __rv = OML_E_NOT_FOUND; \
      break; \
    } \
    typeof(&((this)->elems[(p_it)->pos])) p_list = &((this)->elems[(p_it)->pos]); \
    oml_list_value_type( p_list ) __p;                                  \
    oml_list_get( p_list, &(p_it)->it, &__p );                          \
    *(p_key) = oml_pair_first(&__p);                                    \
    *(p_val) = oml_pair_second(&__p);                                   \
  } while (0); \
  __rv; \
})

#define oml_map_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    printf("pos: %d  max=%d\n", (p_it)->pos, (this)->max_num_elems);                   \
    if ((p_it)->pos == -1) \
      __rv = OML_E_NOT_FOUND; \
    oml_list_next(&(this)->elems[(p_it)->pos], &(p_it)->it); \
    if (oml_list_has_value(&(this)->elems[(p_it)->pos], &(p_it)->it)) \
      break; \
    while (oml_list_size(&(this)->elems[(p_it)->pos]) == 0 && (p_it)->pos < (this)->max_num_elems) \
      (p_it)->pos++;                                                    \
    printf("pos: %d, max=%d\n", (p_it)->pos, (this)->max_num_elems);                   \
    if ((p_it)->pos < (this)->max_num_elems) \
      oml_list_begin(&(this)->elems[(p_it)->pos], &(p_it)->it); \
    else {         \
      __rv = OML_E_NOT_FOUND; \
      (p_it)->pos = -1; \
      break; \
    } \
  } while (0); \
  __rv; \
})

#endif /* __OML_MAP_H__ */
