#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_vector.h"

#include <stdio.h>
#include <stdlib.h>

#define SIZE 4
#define value_type int
#include "test-vector-factory.h"

container_type h;

int values[SIZE];

int main(int argc, char **argv) {
  int v;

  oml_chk_ok_exit(test_vector_init(&h));

  printf("Size of vector: %d\n", oml_vector_size(&h));
  oml_chk_exit(oml_vector_size(&h) == 0);
  oml_chk_ok_exit(oml_vector_push_back(&h, 1024));
  printf("Size of vector: %d\n", oml_vector_size(&h));
  oml_chk_exit(oml_vector_size(&h) == 1);
  oml_chk_exit(oml_vector_front(&h, &v) == OML_OK && v == 1024);
  oml_chk_ok_exit(oml_vector_pop(&h));

  printf("Size of vector: %d\n", oml_vector_size(&h));
  oml_chk_exit(oml_vector_size(&h) == 0);
  oml_chk_exit(oml_vector_empty(&h));

  oml_chk_ok_exit(oml_vector_push_back(&h, 4));
  oml_chk_ok_exit(oml_vector_push_back(&h, 7));
  oml_chk_ok_exit(oml_vector_push_back(&h, 1));
  oml_chk_ok_exit(oml_vector_push_back(&h, 3));
  printf("Size of vector: %d\n", oml_vector_size(&h));
  oml_chk_exit(oml_vector_size(&h) == 4);

  oml_chk_exit((oml_vector_front(&h, &v) == OML_OK) && (v == 3));
  oml_chk_ok_exit(oml_vector_pop(&h));
  oml_chk_exit((oml_vector_front(&h, &v) == OML_OK) && (v == 1));
  oml_chk_ok_exit(oml_vector_pop(&h));
  oml_chk_exit((oml_vector_front(&h, &v) == OML_OK) && (v == 7));
  oml_chk_ok_exit(oml_vector_pop(&h));
  oml_chk_exit((oml_vector_front(&h, &v) == OML_OK) && (v == 4));
  oml_chk_ok_exit(oml_vector_pop(&h));
  printf("Size of vector: %d\n", oml_vector_size(&h));
  oml_chk_exit(oml_vector_size(&h) == 0);
  oml_chk_exit(oml_vector_empty(&h));

  if (oml_vector_pop(&h) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  int i;
  iterator_type it;

  for (i = 0; i < SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_chk_ok_exit(oml_vector_push_back(&h, values[i]));
  }

  oml_chk_exit(oml_vector_size(&h) == SIZE);
  oml_chk_exit(oml_vector_full(&h));

  printf("Vector dump:\n");
  oml_vector_begin(&h, &it);
  for (; oml_vector_has_value(&h, &it); oml_vector_next(&h, &it)) {
    v = oml_vector_value(&h, &it);
    printf("  List elem: %d\n", v);
  }

  for (i = 0; i < SIZE; ++i) {
    oml_chk_exit(! oml_vector_empty(&h));
    oml_chk_ok_exit(oml_vector_front(&h, &v));
    oml_chk_ok_exit(oml_vector_pop(&h));
    printf("Extracted: %d, expected: %d\n", v, values[SIZE - 1 - i]);
    oml_chk_exit(v == values[SIZE - 1 - i]);
  }
  printf("Size of vector: %d\n", oml_vector_size(&h));
  oml_chk_exit(oml_vector_size(&h) == 0);
  oml_chk_exit(oml_vector_empty(&h));

  oml_chk_ok_exit(oml_vector_cleanup(&h));

  oml_chk_exit(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
