#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_list.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_list(int);

oml_list(int) h;

#define SIZE 4
int values[SIZE];

int main(int argc, char **argv) {
  int v;

  oml_chk_ok_exit(oml_list_init(&h));

  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 0);
  oml_chk_ok_exit(oml_list_push_front(&h, 1024));
  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 1);
  oml_chk_exit(oml_list_front(&h, &v) == OML_OK && v == 1024);
  oml_chk_ok_exit(oml_list_pop_front(&h));

  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 0);
  oml_chk_exit(oml_list_empty(&h));

  oml_chk_ok_exit(oml_list_push_front(&h, 4));
  oml_chk_ok_exit(oml_list_push_front(&h, 7));
  oml_chk_ok_exit(oml_list_push_front(&h, 1));
  oml_chk_ok_exit(oml_list_push_front(&h, 3));
  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 4);

  oml_chk_exit((oml_list_front(&h, &v) == OML_OK) && (v == 3));
  oml_chk_ok_exit(oml_list_pop_front(&h));
  oml_chk_exit((oml_list_front(&h, &v) == OML_OK) && (v == 1));
  oml_chk_ok_exit(oml_list_pop_front(&h));
  oml_chk_exit((oml_list_front(&h, &v) == OML_OK) && (v == 7));
  oml_chk_ok_exit(oml_list_pop_front(&h));
  oml_chk_exit((oml_list_front(&h, &v) == OML_OK) && (v == 4));
  oml_chk_ok_exit(oml_list_pop_front(&h));

  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 0);
  oml_chk_exit(oml_list_empty(&h));

  if (oml_list_pop_front(&h) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  int i;
  oml_list_iterator(int) it;

  for (i = 0; i < SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_chk_ok_exit(oml_list_push_front(&h, values[i]));
  }

  printf("List dump:\n");
  oml_list_begin(&h, &it);
  for (; oml_list_has_value(&h, &it); oml_list_next(&h, &it)) {
    oml_chk_ok_exit(oml_list_get_next(&h, &it, &v));
    printf("  List elem: %d\n", v);
  }

  for (i = 0; i < SIZE; ++i) {
    oml_chk_exit(! oml_list_empty(&h));
    oml_chk_ok_exit(oml_list_front(&h, &v));
    oml_chk_exit(oml_list_pop_front(&h) == OML_OK);
    printf("Extracted: %d, expected: %d\n", v, values[SIZE - 1 - i]);
    oml_chk_exit(v == values[SIZE - 1 - i]);
  }
  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 0);
  oml_chk_exit(oml_list_empty(&h));

  oml_chk_ok_exit(oml_list_cleanup(&h));

  oml_chk_exit(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
