#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_map.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct complex_t {
    int x;
    int y;
} COMPLEX;

typedef char* CHARP;
oml_define_map(int, int);
oml_define_map(int, double);
oml_define_map(CHARP, int);
oml_define_map(int, COMPLEX);

oml_map(int, int) h;

#define MAP_BITS 3
#define MAP_SIZE (1 << MAP_BITS)

void test_iter_dump(oml_map(int, int) *p) {
  printf("Map dump:\n");
  oml_map_iterator(int, int) it;
  for (oml_map_begin(p, &it); oml_map_has_value(p, &it); oml_map_next(p, &it)) {
    int k, v;
    oml_chk_ok_exit(oml_map_iterator_get(p, &it, &k, &v));
    printf("  (%d, %d)\n", k, v);
  }
}

#define double_eq_eps(a, b) (((a) - (b)) <= 0.01 && ((b) - (a)) <= 0.01)

void test_map_int() {
  oml_map(int, int) h;
  oml_chk_ok_exit(oml_map_init(&h, MAP_BITS));

  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 0);
  oml_chk_exit(oml_map_empty(&h));
  oml_chk_ok_exit(oml_map_add(&h, 4, 20));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 1);

  oml_chk_ok_exit(oml_map_add(&h, 7, 35));
  oml_chk_ok_exit(oml_map_add(&h, 1, 5));
  oml_chk_ok_exit(oml_map_add(&h, 12, 60));
  oml_chk_ok_exit(oml_map_add(&h, 3, 12));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 5);

  oml_chk_ok_exit(oml_map_add(&h, 4, 15));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 5);

  int v;
  oml_chk_ok_exit(oml_map_get(&h, 4, &v));
  printf("Get val: %d\n", v);
  oml_chk_exit(v == 15);

  int k = 99;
  printf("key %d is not present in map\n", k);
  oml_chk_exit(oml_map_get(&h, k, &v) == OML_E_NOT_FOUND);

  test_iter_dump(&h);

  oml_map_iterator(int, int) it;

  oml_chk_exit(oml_map_find(&h, 99, &it) == OML_E_NOT_FOUND);

  oml_chk_ok_exit(oml_map_find(&h, 60, &it));
  oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &k, &v));
  oml_chk_exit(k == 12);
  oml_chk_exit(v == 60);
  oml_chk_exit(oml_map_value(&h, &it) == 60);
  oml_chk_ok_exit(oml_map_iterator_set(&h, &it, 33));
  oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &k, &v));
  oml_chk_exit(v == 33);
  oml_chk_ok_exit(oml_map_get(&h, 12, &v));
  oml_chk_exit(v == 33);

  oml_chk_exit(oml_map_size(&h) == 5);

  test_iter_dump(&h);

  oml_chk_ok_exit(oml_map_cleanup(&h));
}

void test_map_double() {
  oml_map(int, double) h;
  oml_chk_ok_exit(oml_map_init(&h, MAP_BITS));

  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 0);
  oml_chk_ok_exit(oml_map_add(&h, 20, 4.4));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 1);

  oml_chk_ok_exit(oml_map_add(&h, 35, 7.7));
  oml_chk_ok_exit(oml_map_add(&h, 5, 1.1));
  oml_chk_ok_exit(oml_map_add(&h, 60, 12.12));
  oml_chk_ok_exit(oml_map_add(&h, 12, 3.3));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 5);

  oml_map_iterator(int, double) it;
  int key;
  double val = 0.0;
  oml_chk_ok_exit(oml_map_find(&h, 12.12, &it));
  oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &key, &val));
  oml_chk_exit(key == 60 && val == 12.12);

  oml_map_begin(&h, &it);
  oml_chk_ok_exit(oml_map_find_eq(&h, 12.12 + 0.001, &it, double_eq_eps));
  oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &key, &val));
  oml_chk_exit(key == 60 && val == 12.12);

  oml_map_begin(&h, &it);
  oml_chk_ok_exit(oml_map_find_eq(&h, 12.12 - 0.001, &it, double_eq_eps));
  oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &key, &val));
  oml_chk_exit(key == 60 && val == 12.12);

  printf("Map dump:\n");
  for (oml_map_begin(&h, &it); oml_map_has_value(&h, &it); oml_map_next(&h, &it)) {
    double k;
    int v;
    oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &k, &v));
    printf("  (%g, %d)\n", k, v);
  }

  oml_chk_ok(oml_map_cleanup(&h));
}


void test_map_str() {
  oml_map(CHARP, int) h;    
  oml_chk_ok_exit(oml_map_init(&h, MAP_BITS));

  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 0);
  oml_chk_ok_exit(oml_map_add_eq(&h, "key1", 20, oml_str_eq, oml_str_hash));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 1);

  oml_chk_ok_exit(oml_map_add_eq(&h, "key2", 35, oml_str_eq, oml_str_hash));
  oml_chk_ok_exit(oml_map_add_eq(&h, "key1", 15, oml_str_eq, oml_str_hash));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 2);

  int v;

  oml_chk_ok_exit(oml_map_get_eq(&h, "key1\x0rubbish", &v, oml_str_eq, oml_str_hash));
  printf("Get val: %d\n", v);
  oml_chk_exit(v == 15);

  oml_chk_ok_exit(oml_map_get_eq(&h, "kkey1" + 1, &v, oml_str_eq, oml_str_hash));
  printf("Get val: %d\n", v);
  oml_chk_exit(v == 15);

  char *s = "key1";
  oml_chk_ok_exit(oml_map_get_eq(&h, s, &v, oml_str_eq, oml_str_hash));
  printf("Get val: %d\n", v);
  oml_chk_exit(v == 15);

  printf("key3 is not present in map\n");
  oml_chk_exit(oml_map_get(&h, "key3", &v) == OML_E_NOT_FOUND);

  printf("Map dump:\n");
  oml_map_iterator(CHARP, int) it;
  for (oml_map_begin(&h, &it); oml_map_has_value(&h, &it); oml_map_next(&h, &it)) {
    char* k;
    int v;
    oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &k, &v));
    printf("  (%s, %d)\n", k, v);
  }

  oml_chk_ok(oml_map_cleanup(&h));
}

void test_map_complex() {
  oml_map(int, COMPLEX) h;
  oml_chk_ok_exit(oml_map_init(&h, MAP_BITS));

  COMPLEX c1;
  c1.x = 10;
  c1.y = 11;
  oml_chk_ok_exit(oml_map_add(&h, 1, c1));

  COMPLEX c2;
  oml_chk_ok_exit(oml_map_get(&h, 1, &c2));
  printf("Get val.x: %d\n", c2.x);
  oml_chk_exit(c2.x== 10);
}

int main(int argc, char **argv) {
  test_map_int();
  test_map_double();
  test_map_str();
  test_map_complex();

  printf("Test successful\n");
  return 0;
}
