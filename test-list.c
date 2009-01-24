#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_list.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_list(int);

oml_list(int) h;

#define SIZE 4
int values[SIZE];

/** Insert elements with push_back and verify they are in the same order **/
void test_const_iterator() {
  oml_list(int) l;
  oml_list_iterator(int) it;
  int i, v;

  oml_chk_ok_exit(oml_list_init(&l));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_push_back(&l, values[i]));
    printf("Size of list: %d\n", oml_list_size(&l));
    oml_chk_exit(oml_list_size(&l) == i + 1);
  }
  oml_chk_ok_exit(oml_list_const_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok(oml_list_const_get(&l, &it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == values[i]);
    oml_list_const_next(&l, &it);
  }
  oml_chk_ok_exit(oml_list_cleanup(&l));
}

/** Insert elements with push_front and verify they are in reversed order **/
void test_const_iterator_rev() {
  oml_list(int) l;
  oml_list_iterator(int) it;
  int i, v;

  oml_chk_ok_exit(oml_list_init(&l));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_push_front(&l, values[i]));
    printf("Size of list: %d\n", oml_list_size(&l));
    oml_chk_exit(oml_list_size(&l) == i + 1);
  }
  oml_chk_ok_exit(oml_list_const_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok(oml_list_const_get(&l, &it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == values[SIZE - 1 - i]);
    oml_list_const_next(&l, &it);
  }
  oml_chk_ok_exit(oml_list_cleanup(&l));
}

void test_iterator() {
  oml_list(int) l;
  oml_list_iterator(int) it;
  int i, v;

  oml_chk_ok_exit(oml_list_init(&l));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_push_back(&l, values[i]));
    printf("Size of list: %d\n", oml_list_size(&l));
    oml_chk_exit(oml_list_size(&l) == i + 1);
  }
  oml_chk_ok_exit(oml_list_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok(oml_list_get(&l, &it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == values[i]);
    oml_list_next(&l, &it);
  }
  oml_chk_ok_exit(oml_list_cleanup(&l));
}

void test_iterator_rev() {
  oml_list(int) l;
  oml_list_iterator(int) it;
  int i, v;

  oml_chk_ok_exit(oml_list_init(&l));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_push_front(&l, values[i]));
    printf("Size of list: %d\n", oml_list_size(&l));
    oml_chk_exit(oml_list_size(&l) == i + 1);
  }
  oml_chk_ok_exit(oml_list_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok(oml_list_get(&l, &it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == values[SIZE - 1 - i]);
    oml_list_next(&l, &it);
  }
  oml_chk_ok_exit(oml_list_cleanup(&l));
}

/** Verify elements are in the same order as values[] **/
void test_values_forward(oml_list(int) *pl) {
  oml_list_iterator(int) it;
  int i, v;
  oml_chk_ok_exit(oml_list_const_begin(pl, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok(oml_list_const_get(pl, &it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == values[i]);
    oml_chk_exit(oml_list_const_value(pl, &it) == values[i]);
    oml_list_const_next(pl, &it);
  }
  oml_chk_exit(! oml_list_const_has_value(pl, &it));
}

/** Verify elements are in reversed order w.r.t. values[] **/
void test_values_backward(oml_list(int) *pl) {
  oml_list_iterator(int) it;
  int i, v;
  oml_chk_ok_exit(oml_list_const_begin(pl, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok(oml_list_const_get(pl, &it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == values[SIZE - 1 - i]);
    oml_chk_exit(oml_list_const_value(pl, &it) == values[SIZE - 1 - i]);
    oml_list_const_next(pl, &it);
  }
  oml_chk_exit(! oml_list_const_has_value(pl, &it));
}

/** Insert elements with insert always at begin and always at last, and
 ** verify they are, respectively, in the same and reversed order.
 **
 ** Also, delete elements using an iterator always at begin
 ** and verify they are deleted, respectively, in the same and reversed order.
 **/
void test_insert() {
  oml_list(int) l;
  oml_list_iterator(int) it;
  int i, v;

  oml_chk_ok_exit(oml_list_init(&l));
  oml_chk_ok_exit(oml_list_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_insert(&l, &it, values[i]));
    oml_chk_ok_exit(oml_list_get(&l, &it, &v));
    oml_chk_exit(v == values[i]);
  }
  test_values_backward(&l);

  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_get(&l, &it, &v));
    oml_chk_exit(v == values[SIZE - 1 - i]);
    oml_chk_ok_exit(oml_list_remove(&l, &it));
  }
  oml_chk_exit(oml_list_empty(&l));
  oml_chk_ok_exit(oml_list_cleanup(&l));

  oml_chk_ok_exit(oml_list_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_insert(&l, &it, values[i]));
    oml_chk_ok_exit(oml_list_get(&l, &it, &v));
    oml_chk_exit(v == values[i]);
    oml_chk_ok_exit(oml_list_next(&l, &it));
  }
  test_values_forward(&l);

  oml_chk_ok_exit(oml_list_begin(&l, &it));
  for (i = 0; i < SIZE; ++i) {
    oml_chk_ok_exit(oml_list_get(&l, &it, &v));
    oml_chk_exit(v == values[i]);
    oml_chk_ok_exit(oml_list_remove(&l, &it));
  }
  oml_chk_exit(oml_list_empty(&l));
  oml_chk_ok_exit(oml_list_cleanup(&l));
}

int main(int argc, char **argv) {
  int v;
  int i;

  oml_chk_ok_exit(oml_list_init(&h));

  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 0);

  oml_chk_ok_exit(oml_list_push_front(&h, 1024));
  printf("Size of list: %d\n", oml_list_size(&h));
  oml_chk_exit(oml_list_size(&h) == 1);
  oml_chk_exit(oml_list_front(&h, &v) == OML_OK && (v == 1024));
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

  oml_list_iterator(int) it;

  for (i = 0; i < SIZE; ++i) {
    values[i] = rand();
    printf("Inserting %d\n", values[i]);
    oml_chk_ok_exit(oml_list_push_front(&h, values[i]));
  }

  printf("List dump:\n");
  oml_list_begin(&h, &it);
  for (; oml_list_has_value(&h, &it); oml_list_next(&h, &it)) {
    oml_chk_ok_exit(oml_list_get(&h, &it, &v));
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

  test_iterator();
  test_const_iterator();

  test_iterator_rev();
  test_const_iterator_rev();

  test_insert();
  test_del();

  oml_chk_exit(oml_mem_clean());
  printf("Test successful\n");
  return 0;
}
