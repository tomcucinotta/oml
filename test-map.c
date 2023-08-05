#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_map.h"

#include <stdio.h>
#include <stdlib.h>

typedef char* CHARP;
oml_define_map(int,int);
oml_define_map(double,int);
oml_define_map(CHARP,int);

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

void test_map_double() {
  oml_map(double, int) h;
  oml_chk_ok_exit(oml_map_init(&h, MAP_BITS));

  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 0);
  oml_chk_ok_exit(oml_map_add(&h, 4.4, 20));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 1);

  oml_chk_ok_exit(oml_map_add(&h, 7.7, 35));
  oml_chk_ok_exit(oml_map_add(&h, 1.1, 5));
  oml_chk_ok_exit(oml_map_add(&h, 12.12, 60));
  oml_chk_ok_exit(oml_map_add(&h, 3.3, 12));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 5);


  int val = 0.0;
  oml_chk_ok_exit(oml_map_get_eq(&h, 3.3, &val, double_eq_eps));
  oml_chk_exit(val == 12);
  val = 0.0;
  oml_chk_ok_exit(oml_map_get_eq(&h, 3.3, &val, double_eq_eps));
  oml_chk_exit(val == 12);

  printf("Map dump:\n");
  oml_map_iterator(double, int) it;
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
  oml_chk_ok_exit(oml_map_add_eq(&h, "key1", 20, oml_str_eq));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 1);

  oml_chk_ok_exit(oml_map_add_eq(&h, "key2", 35, oml_str_eq));
  oml_chk_ok_exit(oml_map_add_eq(&h, "key1", 15, oml_str_eq));
  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 2);

  int v;
  oml_chk_ok_exit(oml_map_get_eq(&h, "key1", &v, oml_str_eq));
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

int main(int argc, char **argv) {
  oml_chk_ok_exit(oml_map_init(&h, MAP_BITS));

  printf("Size of map: %d\n", oml_map_size(&h));
  oml_chk_exit(oml_map_size(&h) == 0);
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
  oml_chk_ok_exit(oml_map_iterator_set(&h, &it, 33));
  oml_chk_ok_exit(oml_map_iterator_get(&h, &it, &k, &v));
  oml_chk_exit(v == 33);
  oml_chk_ok_exit(oml_map_get(&h, 12, &v));
  oml_chk_exit(v == 33);

  oml_chk_exit(oml_map_size(&h) == 5);

  test_iter_dump(&h);

  oml_chk_ok_exit(oml_map_cleanup(&h));

  test_map_double();
  test_map_str();

  printf("Test successful\n");
  return 0;
}
