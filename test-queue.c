#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_vqueue.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_vqueue(int);

oml_vqueue(int) h;

#define QUEUE_SIZE 4

int main(int argc, char **argv) {
  int v;

  oml_chk_ok_exit(oml_vqueue_init(&h, QUEUE_SIZE));

  printf("Size of queue: %d\n", oml_vqueue_size(&h));
  oml_chk_exit(oml_vqueue_size(&h) == 0);
  oml_chk_ok_exit(oml_vqueue_push(&h, 1024));
  printf("Size of queue: %d\n", oml_vqueue_size(&h));
  oml_chk_exit(oml_vqueue_size(&h) == 1);
  oml_chk_ok_exit(oml_vqueue_front(&h, &v));
  oml_chk_exit(v == 1024);
  oml_chk_exit(oml_vqueue_pop(&h, &v) == OML_OK && v == 1024);

  printf("Size of queue: %d\n", oml_vqueue_size(&h));
  oml_chk_exit(oml_vqueue_size(&h) == 0);
  oml_chk_exit(oml_vqueue_empty(&h));

  oml_chk_ok_exit(oml_vqueue_push(&h, 4));
  oml_chk_ok_exit(oml_vqueue_push(&h, 7));
  oml_chk_ok_exit(oml_vqueue_push(&h, 1));
  oml_chk_ok_exit(oml_vqueue_push(&h, 3));
  printf("Size of queue: %d\n", oml_vqueue_size(&h));
  oml_chk_exit(oml_vqueue_size(&h) == 4);
  oml_chk_exit(oml_vqueue_full(&h));

  if (oml_vqueue_push(&h, 25) != OML_E_FULL) {
    printf("Should have experienced an error on full heap\n");
    return -1;
  }

  oml_chk_exit((oml_vqueue_front(&h, &v) == OML_OK) && (v == 4));
  oml_chk_exit((oml_vqueue_pop(&h, &v) == OML_OK) && (v == 4));
  oml_chk_exit((oml_vqueue_pop(&h, &v) == OML_OK) && (v == 7));
  oml_chk_exit((oml_vqueue_pop(&h, &v) == OML_OK) && (v == 1));
  oml_chk_exit((oml_vqueue_pop(&h, &v) == OML_OK) && (v == 3));
  printf("Size of queue: %d\n", oml_vqueue_size(&h));
  oml_chk_exit(oml_vqueue_size(&h) == 0);
  oml_chk_exit(oml_vqueue_empty(&h));

  if (oml_vqueue_pop(&h, &v) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  int i;
  int values[QUEUE_SIZE];
  for (i = 0; i < QUEUE_SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_chk_ok_exit(oml_vqueue_push(&h, values[i]));
  }

  oml_vqueue_iterator(int) it;
  oml_vqueue_begin(&h, &it);

  printf("Heap dump:\n");
  while (oml_vqueue_has_value(&h, &it)) {
    oml_chk_ok_exit(oml_vqueue_next(&h, &it, &v));
    printf("  Queue elem: %d\n", v);
  }

  for (i = 0; i < QUEUE_SIZE; ++i) {
    oml_chk_ok_exit(oml_vqueue_front(&h, &v));
    printf("Front: %d (expected: %d)\n", v, values[i]);
    oml_chk_exit(v == values[i]);
    oml_chk_exit((oml_vqueue_pop(&h, &v) == OML_OK) && (v == values[i]));
  }
  printf("Size of queue: %d\n", oml_vqueue_size(&h));
  oml_chk_exit(oml_vqueue_size(&h) == 0);
  oml_chk_exit(oml_vqueue_empty(&h));

  oml_chk_ok_exit(oml_vqueue_cleanup(&h));

  oml_chk_exit(oml_mem_clean());

  printf("Test successful\n");
  return 0;
}
