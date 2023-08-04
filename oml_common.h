#ifndef _OML_COMMON_H_
#define _OML_COMMON_H_

#include <stdint.h>
#include <string.h>

/** All ordered data types in OML use this default macro in order
 ** to compare sorting keys, unless the specific additional operations
 ** with the "_lt" suffix are used when modifying contents. Therefore,
 ** all ordered data types are sorted from the smallest to the greatest
 ** value, according to the "<" ordering.
 ** 
 ** You can redefine the default ordering for all OML types
 ** by defining the oml_default_lt macro to something different
 ** from the operator "<" before including any OML include file.
 **/
#ifndef oml_default_lt
#  define oml_default_lt(a, b) ((a) < (b))
#endif

#ifndef oml_default_eq
#  define oml_default_eq(a, b) ((a) == (b))
#endif

#ifndef oml_default_eq_dbg
#  define oml_default_eq_dbg(a, b) (printf("Comparing %d and %d\n", (a), (b)), (a) == (b))
#endif

#ifndef oml_str_eq
#  define oml_str_eq(a, b) ( strcmp(a,b) == 0 )
#endif

#ifndef oml_str_eq_dbg
#  define oml_str_eq_dbg(a, b) ( printf("Comparing %s and %s\n", (a), (b)), strcmp(a,b) == 0 )
#endif
/** A quite good hash function (according to Wikipedia) **/
static inline uint32_t joaat_hash_func(uint8_t *key, size_t len) {
  uint32_t hash = 0;
  size_t i;

  for (i = 0; i < len; i++) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

#ifndef oml_default_hash_func
#  define oml_default_hash_func joaat_hash_func
#endif

#ifndef oml_default_hash
#  define oml_default_hash(_key) oml_default_hash_func((uint8_t *) &(_key), sizeof(_key))
#endif

#endif /*_OML_COMMON_H_ */
