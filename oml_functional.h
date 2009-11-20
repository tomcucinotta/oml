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

/** Swap **/
#define oml_swap(a, b)				\
  do {						\
    typeof(a) __oml_tmp = (a);			\
    (a) = (b);					\
    (b) = __oml_tmp;				\
  } while (0)

/** Min **/
#define oml_min(a, b) ({			\
      typeof(a) __oml_a = (a);			\
      typeof(b) __oml_b = (b);			\
      (__oml_a < __oml_b ? __oml_a : __oml_b);	\
    })

/** Max **/
#define oml_max(a, b) ({			\
      typeof(a) __oml_a = (a);			\
      typeof(b) __oml_b = (b);			\
      (__oml_a > __oml_b ? __oml_a : __oml_b);	\
    })

#endif /* __OML_FUNCTIONAL_H__ */
