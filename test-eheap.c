#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_eheap.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_eheap(int, int);

oml_eheap(int, int) h;

#define HEAP_BITS 3
#define HEAP_SIZE (1 << HEAP_BITS)

int main(int argc, char **argv) {
  int k, v;

  oml_assert_ok(oml_eheap_init(&h, HEAP_BITS));

  printf("Size of heap: %d\n", oml_eheap_size(&h));
  oml_assert(oml_eheap_size(&h) == 0);
  oml_assert_ok(oml_eheap_add(&h, 4, 20, NULL));
  printf("Size of heap: %d\n", oml_eheap_size(&h));
  oml_assert(oml_eheap_size(&h) == 1);

  oml_assert_ok(oml_eheap_add(&h, 7, 35, NULL));
  oml_assert_ok(oml_eheap_add(&h, 1, 5, NULL));
  oml_assert_ok(oml_eheap_add(&h, 12, 60, NULL));
  oml_assert_ok(oml_eheap_add(&h, 3, 12, NULL));
  printf("Size of heap: %d\n", oml_eheap_size(&h));
  oml_assert(oml_eheap_size(&h) == 5);

  oml_assert((oml_eheap_get_min(&h, &k, &v) == OML_OK) && (k == 1) && (v == 5));
  oml_assert_ok(oml_eheap_del_min(&h));
  oml_assert((oml_eheap_get_min(&h, &k, &v) == OML_OK) && (k == 3) && (v == 12));
  oml_assert_ok(oml_eheap_del_min(&h));
  oml_assert((oml_eheap_get_min(&h, &k, &v) == OML_OK) && (k == 4) && (v == 20));
  oml_assert_ok(oml_eheap_del_min(&h));
  oml_assert((oml_eheap_get_min(&h, &k, &v) == OML_OK) && (k == 7) && (v == 35));
  oml_assert_ok(oml_eheap_del_min(&h));
  oml_assert((oml_eheap_get_min(&h, &k, &v) == OML_OK) && (k == 12) && (v == 60));
  oml_assert_ok(oml_eheap_del_min(&h));

  oml_assert_ok(oml_eheap_add(&h, 7, 35, NULL));
  oml_assert_ok(oml_eheap_add(&h, 1, 5, NULL));
  oml_assert_ok(oml_eheap_add(&h, 12, 60, NULL));
  oml_assert_ok(oml_eheap_add(&h, 3, 12, NULL));
  oml_assert_ok(oml_eheap_add(&h, 4, 20, NULL));

  printf("Heap dump:\n");
  while (oml_eheap_size(&h) > 0) {
    oml_assert_ok(oml_eheap_get_min(&h, &k, &v));
    printf("Min heap elem: %d, %d\n", k, v);
    oml_assert_ok(oml_eheap_del_min(&h));
  }

  oml_eheap_iterator(int, int) it1, it2, it3;

  printf("Rebuilding heap...\n");

  oml_assert_ok(oml_eheap_add(&h, 7, 35, &it1));
  oml_assert_ok(oml_eheap_add(&h, 1, 5, &it2));
  oml_assert_ok(oml_eheap_add(&h, 5, 25, &it3));

  printf("Deleting random elem...\n");

  oml_assert_ok(oml_eheap_del(&h, &it3));

  printf("Heap dump:\n");
  while (oml_eheap_size(&h) > 0) {
    oml_assert_ok(oml_eheap_get_min(&h, &k, &v));
    printf("Min heap elem: %d, %d\n", k, v);
    oml_assert_ok(oml_eheap_del_min(&h));
  }

  int i;
  for (i = 0; i < HEAP_SIZE; i++) {
    int rnd = 1 + (int) (100.0 * (rand() / (RAND_MAX + 1.0)));
    oml_assert_ok(oml_eheap_add(&h, rnd, rnd, NULL));
  }
  if (oml_eheap_add(&h, 5, 25, NULL) != OML_E_FULL) {
    printf("Should have experienced an error on full heap\n");
    return -1;
  }

  for (i = 0; i < HEAP_SIZE; i++)
    oml_assert_ok(oml_eheap_del_min(&h));

  if (oml_eheap_del_min(&h) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  oml_assert_ok(oml_eheap_cleanup(&h));

  printf("Test successful\n");
  return 0;
}
