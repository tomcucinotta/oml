#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_vstack.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_vstack(int);

oml_vstack(int) h;

#define SIZE 4
int values[SIZE];

int main(int argc, char **argv) {
  int v;

  oml_chk_ok_exit(oml_vstack_init(&h, SIZE));

  printf("Size of vstack: %d\n", oml_vstack_size(&h));
  oml_chk_exit(oml_vstack_size(&h) == 0);
  oml_chk_ok_exit(oml_vstack_push(&h, 1024));
  printf("Size of vstack: %d\n", oml_vstack_size(&h));
  oml_chk_exit(oml_vstack_size(&h) == 1);
  oml_chk_exit(oml_vstack_front(&h, &v) == OML_OK && v == 1024);
  oml_chk_ok_exit(oml_vstack_pop(&h));

  printf("Size of vstack: %d\n", oml_vstack_size(&h));
  oml_chk_exit(oml_vstack_size(&h) == 0);
  oml_chk_exit(oml_vstack_empty(&h));

  oml_chk_ok_exit(oml_vstack_push(&h, 4));
  oml_chk_ok_exit(oml_vstack_push(&h, 7));
  oml_chk_ok_exit(oml_vstack_push(&h, 1));
  oml_chk_ok_exit(oml_vstack_push(&h, 3));
  printf("Size of vstack: %d\n", oml_vstack_size(&h));
  oml_chk_exit(oml_vstack_size(&h) == 4);

  oml_chk_exit((oml_vstack_front(&h, &v) == OML_OK) && (v == 3));
  oml_chk_ok_exit(oml_vstack_pop(&h));
  oml_chk_exit((oml_vstack_front(&h, &v) == OML_OK) && (v == 1));
  oml_chk_ok_exit(oml_vstack_pop(&h));
  oml_chk_exit((oml_vstack_front(&h, &v) == OML_OK) && (v == 7));
  oml_chk_ok_exit(oml_vstack_pop(&h));
  oml_chk_exit((oml_vstack_front(&h, &v) == OML_OK) && (v == 4));
  oml_chk_ok_exit(oml_vstack_pop(&h));
  printf("Size of vstack: %d\n", oml_vstack_size(&h));
  oml_chk_exit(oml_vstack_size(&h) == 0);
  oml_chk_exit(oml_vstack_empty(&h));

  if (oml_vstack_pop(&h) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  int i;
  oml_vstack_iterator(int) it;

  for (i = 0; i < SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_chk_ok_exit(oml_vstack_push(&h, values[i]));
  }

  oml_chk_exit(oml_vstack_size(&h) == SIZE);
  oml_chk_exit(oml_vstack_full(&h));

  printf("VStack dump:\n");
  oml_vstack_begin(&h, &it);
  for (; oml_vstack_has_next(&h, &it); oml_vstack_next(&h, &it)) {
    oml_chk_ok_exit(oml_vstack_get_next(&h, &it, &v));
    printf("  List elem: %d\n", v);
  }

  for (i = 0; i < SIZE; ++i) {
    oml_chk_exit(! oml_vstack_empty(&h));
    oml_chk_ok_exit(oml_vstack_front(&h, &v));
    oml_chk_ok_exit(oml_vstack_pop(&h));
    printf("Extracted: %d, expected: %d\n", v, values[SIZE - 1 - i]);
    oml_chk_exit(v == values[SIZE - 1 - i]);
  }
  printf("Size of vstack: %d\n", oml_vstack_size(&h));
  oml_chk_exit(oml_vstack_size(&h) == 0);
  oml_chk_exit(oml_vstack_empty(&h));

  oml_chk_ok_exit(oml_vstack_cleanup(&h));

  oml_chk_exit(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
