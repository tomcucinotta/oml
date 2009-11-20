#ifndef OML_CLASS_SERIALIZE_H_
#define OML_CLASS_SERIALIZE_H_

/** @file
 ** @brief Support macros for Object Oriented code.
 **
 ** Sample usage:
 **
 ** oml_class_declare(myClass) {
 **   oml_vtable(myClass);		// Optional, class has virtual methos
 **   oml_class_extend(parentClass);	// Optional
 **   oml_class_member(int, x);		// Member declaration
 ** };
 **
 ** oml_class_vtable(myclass) {
 **   oml_vmeth_declare(myClass, int, f, int x, int y);	// Virtual method declaration
 ** };
 **
 ** oml_meth_declare(myClass, int, g, int x, int y);	// Non-virtual method decl
 ** oml_class_constructor(myClass) {	// VTable automatically set-up ?
 **   ...
 ** }
 **
 ** myClass *c = oml_new(myClass);	// Constructor automatically called.
 ** int x, y, z;
 ** z = oml_call(c, g, x, y);	// Non-virtual call
 ** z = oml_vcall(c, f, x, y);	// Virtual call
 ** 
 **/

#include <assert.h>
#include <string.h>

#undef oml_class_declare
#undef oml_class_vtable
#undef oml_vtable
#undef oml_class_extend
#undef oml_class_member
#undef oml_meth_declare

#define oml_class_declare(class_name) \
  void serialize_##class_name(class_name *this, unsigned char **p_buf, unsigned long *p_size)

#define oml_class_vtable(class_name)

#define oml_vtable(class_name)

/** With ms-extensions enabled, an unnamed struct could be used here **/
#define oml_class_extend(base_type) \
  serialize_##base_type(&this->base, p_buf, p_size)

#define oml_class_member(type, var) \
  serialize_##type(&this->var, p_buf, p_size)

#define oml_meth_declare(class, rv, meth_name, args...)

#define write_var(p_buf, p_size, v)                             \
  do {                                                          \
    assert(*p_size >= sizeof(v));                               \
    memcpy(*p_buf, &v, sizeof(v));                              \
    *p_size -= sizeof(v);                                       \
    *p_buf += sizeof(v);                                        \
  } while (0)

#define read_var(p_buf, p_size, v)                              \
  do {                                                          \
    assert(*p_size >= sizeof(v));                               \
    memcpy(&v, *p_buf, sizeof(v));                              \
    *p_size -= sizeof(v);                                       \
    *p_buf += sizeof(v);                                        \
  } while (0)

static inline void serialize_int(int *p_value, unsigned char **p_buf, unsigned long *p_size) {
  write_var(p_buf, p_size, *p_value);
}

static inline void serialize_long(long *p_value, unsigned char **p_buf, unsigned long *p_size) {
  write_var(p_buf, p_size, *p_value);
}

static inline void serialize_double(double *p_value, unsigned char **p_buf, unsigned long *p_size) {
  write_var(p_buf, p_size, *p_value);
}

#endif /* OML_CLASS_SERIALIZE_H_ */
