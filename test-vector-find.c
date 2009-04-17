#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_vector_find.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_vector(int);

oml_vector(int) h;

#define SIZE 4
int values[SIZE];

int main(int argc, char **argv) {
  int i;
  oml_vector_iterator(int) it;

  oml_chk_ok_exit(oml_vector_init(&h, SIZE));
  for (i = 0; i < SIZE; ++i)
    oml_chk_ok_exit(oml_vector_push_back(&h, i));

  for (i = 0; i < SIZE; ++i) {
    int v = -1;
    oml_vector_iterator(int) end;
    oml_vector_begin(&h, &it);
    oml_vector_end(&h, &end);
    oml_log_debug("i=%d", i);
    oml_chk_ok_exit(oml_vector_next_n(&h, &it, i));
    oml_chk_ok_exit(oml_vector_get(&h, &it, &v));
    oml_chk_exit(v == i);
  }

  oml_log_debug("Searching for %d", SIZE);
  oml_chk_exit(oml_vector_find(&h, SIZE, &it) == OML_E_NOT_FOUND);
  oml_log_debug("Finished");

  for (i = 0; i < SIZE; ++i) {
    int v = -1;
    oml_vector_iterator(int) end;
    oml_vector_begin(&h, &it);
    oml_vector_end(&h, &end);
    oml_log_debug("Searching for %d", i);
    oml_chk_ok_exit(oml_vector_find_in(&h, i, &it, &end));
    oml_log_debug("Finished");
    oml_chk_ok_exit(oml_vector_get(&h, &it, &v));
    oml_chk_exit(v == i);
  }

  oml_chk_ok_exit(oml_vector_cleanup(&h));

  oml_chk_exit(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
