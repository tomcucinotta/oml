#include <stdlib.h>

#undef oml_class_declare
#undef oml_class_vtable
#undef oml_vtable
#undef oml_class_extend
#undef oml_class_member
#undef oml_meth_declare

#define oml_class_declare(class_name)		\
  void class_name ##_init(class_name *this);	\
  class_name * class_name ##_new() {	\
    class_name *p = malloc(sizeof(class_name)); \
    class_name ##_init(p);			\
    return p;					\
  }						\
  void class_name ##_init(class_name *this)

#define oml_class_vtable(class_name)

#define oml_vtable(class_name)

#define oml_class_extend(base_type) \
  base_type ##_init(&this->base)

#define oml_class_member(type, var) \
  type ##_new(&this->var)

#define oml_meth_declare(class, rv, meth_name, args...)

static inline void int_new(int *p_value) { *p_value = 0; }
static inline void long_new(long *p_value) { *p_value = 0; }
static inline void double_new(double *p_value) { *p_value = 0; }
