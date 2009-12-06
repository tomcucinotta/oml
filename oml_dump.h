#ifndef __OML_VECTOR_DUMP_H__
#define __OML_VECTOR_DUMP_H__

#include "oml_foreach.h"

/** Dumps entire contents of container.
 **
 ** Example usage:
 **   oml_vector(int) v;
 **   ...
 **   oml_dump(vector, &v);
 **/
#define oml_dump(type, self)			\
  do {						\
    oml_##type##_iterator(int) it, end;		\
    oml_##type##_begin(self, &it);		\
    oml_##type##_end(self, &end);		\
    printf("(");				\
    oml_foreach(type, self, &it, &end) {	\
      int val;					\
      oml_##type##_get(self, &it, &val);	\
      printf("%d, ", val);			\
    }						\
    printf(")\n");				\
  } while (0)

#endif /* __OML_VECTOR_DUMP_H__ */
