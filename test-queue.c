#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_queue.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_queue(int);

oml_queue(int) h;

#define QUEUE_SIZE 4

int main(int argc, char **argv) {
  int v;

  oml_assert_ok(oml_queue_init(&h, QUEUE_SIZE));

  printf("Size of queue: %d\n", oml_queue_size(&h));
  oml_assert(oml_queue_size(&h) == 0);
  oml_assert_ok(oml_queue_push(&h, 1024));
  printf("Size of queue: %d\n", oml_queue_size(&h));
  oml_assert(oml_queue_size(&h) == 1);
  oml_assert_ok(oml_queue_front(&h, &v));
  oml_assert(v == 1024);
  oml_assert(oml_queue_pop(&h, &v) == OML_OK && v == 1024);

  printf("Size of queue: %d\n", oml_queue_size(&h));
  oml_assert(oml_queue_size(&h) == 0);
  oml_assert(oml_queue_empty(&h));

  oml_assert_ok(oml_queue_push(&h, 4));
  oml_assert_ok(oml_queue_push(&h, 7));
  oml_assert_ok(oml_queue_push(&h, 1));
  oml_assert_ok(oml_queue_push(&h, 3));
  printf("Size of queue: %d\n", oml_queue_size(&h));
  oml_assert(oml_queue_size(&h) == 4);
  oml_assert(oml_queue_full(&h));

  if (oml_queue_push(&h, 25) != OML_E_FULL) {
    printf("Should have experienced an error on full heap\n");
    return -1;
  }

  oml_assert((oml_queue_front(&h, &v) == OML_OK) && (v == 4));
  oml_assert((oml_queue_pop(&h, &v) == OML_OK) && (v == 4));
  oml_assert((oml_queue_pop(&h, &v) == OML_OK) && (v == 7));
  oml_assert((oml_queue_pop(&h, &v) == OML_OK) && (v == 1));
  oml_assert((oml_queue_pop(&h, &v) == OML_OK) && (v == 3));
  printf("Size of queue: %d\n", oml_queue_size(&h));
  oml_assert(oml_queue_size(&h) == 0);
  oml_assert(oml_queue_empty(&h));

  if (oml_queue_pop(&h, &v) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  int i;
  int values[QUEUE_SIZE];
  for (i = 0; i < QUEUE_SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_assert_ok(oml_queue_push(&h, values[i]));
  }

  oml_queue_iterator(int) it;
  oml_queue_begin(&h, &it);

  printf("Heap dump:\n");
  while (oml_queue_has_next(&h, &it)) {
    oml_assert_ok(oml_queue_next(&h, &it, &v));
    printf("  Queue elem: %d\n", v);
  }

  for (i = 0; i < QUEUE_SIZE; ++i) {
    oml_assert_ok(oml_queue_front(&h, &v));
    printf("Front: %d (expected: %d)\n", v, values[i]);
    oml_assert(v == values[i]);
    oml_assert((oml_queue_pop(&h, &v) == OML_OK) && (v == values[i]));
  }
  printf("Size of queue: %d\n", oml_queue_size(&h));
  oml_assert(oml_queue_size(&h) == 0);
  oml_assert(oml_queue_empty(&h));

  oml_assert_ok(oml_queue_cleanup(&h));

  oml_assert(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
