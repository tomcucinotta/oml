#ifndef OML_CLASS_H_
#define OML_CLASS_H_

/** @file
 ** @brief Support macros for Object Oriented code.
 **
 ** Sample usage:
 **
 ** oml_class_declare(myClass) {
 **   oml_class_extend(parentClass);	// Optional
 **   int x;
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



#define oml_class_declare(class_name) \
  typedef struct class_name class_name; \
  struct class_name 

/** With ms-extensions enabled, an unnamed struct could be used here **/
#define oml_class_extend(base_type) base_type base

#define oml_meth_declare(class, rv, meth_name, args...) \
  oml_func_declare(rv, meth_name, class *this, args)

#define oml_meth_define(class, rv, meth_name, args...) \
  oml_func_define(rv, meth_name, class *this, args)

#define oml_meth_call(instance, method, args...) \
  method(instance, args)

#endif /* OML_CLASS_H_ */
