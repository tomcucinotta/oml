#ifndef __OML_SYNC_MAP_H__
#define __OML_SYNC_MAP_H__

/** @file
 ** 
 ** @brief Generic, thread-safe hashmap.
 ** 
 ** @note
 ** If you need to hash keys with anything different from the oml_default_hash()
 ** function, then
 **/

#include "oml_sync.h"
#include "oml_map.h"

#include "oml_debug.h"
#include "oml_common.h"

/** Define sync map types: use this macro only once per compilation unit */
#define oml_define_sync_map(key_type, value_type) \
  typedef struct oml_sync_map_##key_type ##_##value_type { \
    oml_map(key_type, value_type) map; \
    oml_mutex mtx; \
  } oml_sync_map_##key_type ##_##value_type; \

/** Template-like type for a synchronized map container */
#define oml_sync_map(key_type, value_type) \
  oml_sync_map_##key_type ##_##value_type


/** Initialize an empty sync map with a hashtable size of 2 ^ N **/
#define oml_sync_map_init(this, N) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    __rv = oml_map_init(&(this)->map, N); \
    oml_mutex_init(&(this)->mtx); \
  } while (0); \
  __rv; \
})

#define oml_sync_map_cleanup(this) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    __rv = oml_map_cleanup(&(this)->map); \
    oml_mutex_destroy(&(this)->mtx); \
  } while (0); \
  __rv; \
})

/**
 ** Add the pair (_key, _value) to the map
 **
 ** @note Adding a second time the same key to the map results in replacing the previously associated value.
 **/
#define oml_sync_map_add_eq(this, _key, _value, _op_key_eq, _op_key_hash) ({      \
  oml_rv __rv = OML_OK; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_map_add_eq(&(this)->map, _key, _value, _op_key_eq, _op_key_hash); \
    } \
  } while (0); \
  __rv; \
})


/**
 * Add the pair (_key, _value) to the map, and store the corresponding
 * removal iterator in *_p_it.
 */
#define oml_sync_map_add(this, _key, _value) \
  oml_sync_map_add_eq((this), (_key), (_value), oml_default_eq, oml_default_hash)


/**
 ** Write corresponding value of _key in p_value
 **/
#define oml_sync_map_get_eq(this, _key, p_value, _op_key_eq, _op_key_hash) ({     \
  oml_rv __rv = OML_OK; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_map_get_eq(&(this)->map, _key, p_value, _op_key_eq, _op_key_hash); \
    } \
  } while (0); \
  __rv; \
})

#define oml_sync_map_get(this, _key, p_value) \
  oml_sync_map_get_eq((this), (_key), (p_value), oml_default_eq, oml_default_hash)

/** Return the current size of the map */
#define oml_sync_map_size(this) ({     \
  int size = 0; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      size = oml_map_size(&(this)->map); \
    } \
  } while (0); \
  size; \
})

#define oml_sync_map_empty(this) ({     \
  int is_empty = 0; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      is_empty = (oml_map_size(&(this)->map) == 0); \
    } \
  } while (0); \
  is_empty; \
})

#define oml_sync_map_begin(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_map_begin(&(this)->map, p_it); \
    } \
  } while (0); \
  __rv; \
})

#define oml_sync_map_has_value(this, p_it) ({     \
  int has_value = 0; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      has_value = ((p_it)->pos != -1); \
    } \
  } while (0); \
  has_value; \
})

#define oml_sync_map_iterator_get(this, p_it, p_key, p_val) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_map_iterator_get(&(this)->map, p_it, p_key, p_val); \
    } \
  } while (0); \
  __rv; \
})

#define oml_sync_map_iterator_set(this, p_it, value) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_map_iterator_set(&(this)->map, p_it, value); \
    } \
  } while (0); \
  __rv; \
})

#define oml_sync_map_next(this, p_it) ({ \
  oml_rv __rv = OML_OK; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_sync_map_next(&(this)->map, p_it); \
    } \
  } while (0); \
  __rv; \
})

#define oml_sync_map_find_eq(this, value, p_it, f_eq) ({  \
  oml_rv __rv = OML_E_NOT_FOUND; \
  do { \
    oml_sync_region(&(this)->mtx) { \
      __rv = oml_map_find_eq(&(this)->map, value, p_it, f_eq); \
    } \
  } while (0); \
  __rv; \
})

#define oml_sync_map_find(this, value, p_it) \
  (oml_sync_map_find_eq((this), (value), (p_it), oml_default_eq))

#endif /* __OML_SYNC_MAP_H__ */
