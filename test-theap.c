#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_theap.h"

#include <stdio.h>
#include <stdlib.h>

//oml_define_theap(int, int);

oml_theap(int, int) h;

void dump() {
  oml_theap_iterator(int, int) it;
  oml_theap_iterator_begin(&h, &it);
  while (! oml_theap_iterator_eoi(&it)) {
    printf(
      "Key: %d, Value: %d\n",
      oml_theap_iterator_get_key(&it),
      oml_theap_iterator_get_value(&it)
    );
  }
}

#define N 10

int buf[N];
int size = 0;

void add(int n) {
  buf[size++] = n;
}

int get_min(void) {
  int m = buf[0];
  int j;
  oml_assert(size > 0);
  for (j = 1; j < size; ++j)
    if (buf[j] < m)
      m = buf[j];
  return m;
}

void del_min(void) {
  int m = 0;
  int j;
  oml_assert(size > 0);
  for (j = 1; j < size; ++j)
    if (buf[j] < buf[m])
      m = j;
  buf[m] = buf[--size];
}

int main(int argc, char **argv) {
  oml_rv rv;
  int k, i;
  char v;

  oml_theap_init(&h);
  oml_theap_dump(&h);
  oml_assert(oml_theap_add(&h, 7, 7) == OML_OK);
  oml_theap_dump(&h);
  oml_assert(oml_theap_add(&h, 3, 3) == OML_OK);
  oml_theap_dump(&h);
  oml_assert(oml_theap_add(&h, 1, 1) == OML_OK);
  oml_theap_dump(&h);
  oml_assert(oml_theap_add(&h, 12, 12) == OML_OK);
  oml_theap_dump(&h);
  oml_assert(oml_theap_add(&h, 5, 5) == OML_OK);
  oml_theap_dump(&h);

  while (oml_theap_size(&h) > 0) {
    oml_assert(oml_theap_get_min(&h, &k, &v) == OML_OK);
    printf("Min heap elem: %d, %d\n", k, v);
    oml_assert(oml_theap_del_min(&h) == OML_OK);
    oml_theap_dump(&h);
  }

  for (i = 0; i < N; i++) {
    int rnd = 1 + (int) (100.0 * (rand() / (RAND_MAX + 1.0)));
    rv = oml_theap_add(&h, rnd, rnd);
    add(rnd);
    oml_theap_dump(&h);
    if (rv != OML_OK) {
      printf("Error while inserting\n");
      return -1;
    }
  }
  for (i = 0; i < N; ++i) {
    int k, v, m;
    oml_assert(oml_theap_get_min(&h, &k, &v) == OML_OK);
    m = get_min();
    oml_log_debug("m=%d, k=%d, v=%d", m, k, v);
    oml_assert(k == v && v == m);
    oml_theap_dump(&h);
    oml_assert(oml_theap_del_min(&h) == OML_OK);
    del_min();
  }
  rv = oml_theap_del_min(&h);
  if (rv != OML_E_EMPTY) {
    printf("Should have experienced an error on empty heap\n");
    return -1;
  }

  oml_theap_cleanup(&h);

  printf("Test successful\n");

  return 0;
}
