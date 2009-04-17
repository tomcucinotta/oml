#include <oml_scope.h>
#include <oml_list.h>

#include <stdio.h>

oml_define_list(int);
typedef oml_list(int) (*__oml_aux_f)(oml_list(int) *);

/** No matter what the actual type of the list is */
static void __oml_destroy_list(oml_list(int) *this) {
  printf("Destroying list...\n");
  oml_list_clear(this);
}

/** @todo Raise exception if malloc() fails */
#define oml_list_scoped_init(this)					\
	do {								\
		oml_list_init(this);					\
		oml_scope_add((p_oml_destructor) &__oml_destroy_list, (this)); \
	} while (0)
