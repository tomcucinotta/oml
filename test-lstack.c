#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_lstack.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_lstack(int);

oml_lstack(int) h;

#define SIZE 4
int values[SIZE];

int main(int argc, char **argv) {
  int v;

  oml_chk_ok_exit(oml_lstack_init(&h));

  printf("Size of lstack: %d\n", oml_lstack_size(&h));
  oml_chk_exit(oml_lstack_size(&h) == 0);
  oml_chk_ok_exit(oml_lstack_push(&h, 1024));
  printf("Size of lstack: %d\n", oml_lstack_size(&h));
  oml_chk_exit(oml_lstack_size(&h) == 1);
  oml_chk_exit(oml_lstack_front(&h, &v) == OML_OK && v == 1024);
  oml_chk_ok_exit(oml_lstack_pop(&h));

  printf("Size of lstack: %d\n", oml_lstack_size(&h));
  oml_chk_exit(oml_lstack_size(&h) == 0);
  oml_chk_exit(oml_lstack_empty(&h));

  oml_chk_ok_exit(oml_lstack_push(&h, 4));
  oml_chk_ok_exit(oml_lstack_push(&h, 7));
  oml_chk_ok_exit(oml_lstack_push(&h, 1));
  oml_chk_ok_exit(oml_lstack_push(&h, 3));
  printf("Size of lstack: %d\n", oml_lstack_size(&h));
  oml_chk_exit(oml_lstack_size(&h) == 4);

  oml_chk_exit((oml_lstack_front(&h, &v) == OML_OK) && (v == 3));
  oml_chk_ok_exit(oml_lstack_pop(&h));
  oml_chk_exit((oml_lstack_front(&h, &v) == OML_OK) && (v == 1));
  oml_chk_ok_exit(oml_lstack_pop(&h));
  oml_chk_exit((oml_lstack_front(&h, &v) == OML_OK) && (v == 7));
  oml_chk_ok_exit(oml_lstack_pop(&h));
  oml_chk_exit((oml_lstack_front(&h, &v) == OML_OK) && (v == 4));
  oml_chk_ok_exit(oml_lstack_pop(&h));
  printf("Size of lstack: %d\n", oml_lstack_size(&h));
  oml_chk_exit(oml_lstack_size(&h) == 0);
  oml_chk_exit(oml_lstack_empty(&h));

  if (oml_lstack_pop(&h) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  int i;
  oml_lstack_iterator(int) it;

  for (i = 0; i < SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_chk_ok_exit(oml_lstack_push(&h, values[i]));
  }

  oml_chk_exit(oml_lstack_size(&h) == SIZE);

  printf("VStack dump:\n");
  oml_lstack_begin(&h, &it);
  for (; oml_lstack_has_value(&h, &it); oml_lstack_next(&h, &it)) {
    oml_chk_ok_exit(oml_lstack_get(&h, &it, &v));
    printf("  List elem: %d\n", v);
  }

  for (i = 0; i < SIZE; ++i) {
    oml_chk_exit(! oml_lstack_empty(&h));
    oml_chk_ok_exit(oml_lstack_front(&h, &v));
    oml_chk_ok_exit(oml_lstack_pop(&h));
    printf("Extracted: %d, expected: %d\n", v, values[SIZE - 1 - i]);
    oml_chk_exit(v == values[SIZE - 1 - i]);
  }
  printf("Size of lstack: %d\n", oml_lstack_size(&h));
  oml_chk_exit(oml_lstack_size(&h) == 0);
  oml_chk_exit(oml_lstack_empty(&h));

  oml_chk_ok_exit(oml_lstack_cleanup(&h));

  oml_chk_exit(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
