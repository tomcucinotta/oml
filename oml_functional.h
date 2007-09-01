#ifndef __OML_FUNCTIONAL_H__
#define __OML_FUNCTIONAL_H__

/** Binary functions **/

#define oml_add(x, y) ((x) + (y))
#define oml_sub(x, y) ((x) - (y))
#define oml_mul(x, y) ((x) * (y))
#define oml_div(x, y) ((x) / (y))
#define oml_mod(x, y) ((x) % (y))

/** Unary functions **/

#define oml_neg(x) (-(x))

/** Bitwise operations **/

#define oml_bw_and(x, y) ((x) & (y))
#define oml_bw_or(x, y) ((x) | (y))
#define oml_bw_not(x, y) (~(x))

/** Binary predicates **/

#define oml_eq(x, y) ((x) == (y))
#define oml_ne(x, y) ((x) != (y))
#define oml_lt(x, y) ((x) < (y))
#define oml_le(x, y) ((x) <= (y))
#define oml_gt(x, y) ((x) > (y))
#define oml_ge(x, y) ((x) >= (y))

/** Logical predicates **/

#define oml_and(x, y) ((x) && (y))
#define oml_or(x, y) ((x) || (y))
#define oml_not(x) (! (x))

#endif /* __OML_FUNCTIONAL_H__ */
