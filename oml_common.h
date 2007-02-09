#ifndef _OML_COMMON_H_
#define _OML_COMMON_H_

/**
 * All ordered data types in OML use this default macro in order
 * to compare sorting keys, unless the specific additional operations
 * with the "_lt" suffix are used when modifying contents. Therefore,
 * all ordered data types are sorted from the smallest to the greatest
 * value, according to the "<" ordering.
 * 
 * You can redefine the default ordering for all OML types
 * by defining the oml_default_lt macro to something different
 * from the operator "<" before including any OML include file.
 */
#ifndef oml_default_lt
#  define oml_default_lt(a, b) ((a) < (b))
#endif

#ifndef oml_default_eq
#  define oml_default_eq(a, b) ((a) == (b))
#endif

#endif /*_OML_COMMON_H_ */
