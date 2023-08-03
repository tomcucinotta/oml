#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_map.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_map(int,int);

oml_map(int, int) h;

#define MAP_BITS 3
#define MAP_SIZE (1 << MAP_BITS)

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

  printf("Map dump:\n");
  oml_map_iterator(int, int) it;
  for (oml_map_begin(&h, &it); oml_map_has_value(&h, &it); oml_map_next(&h, &it)) {
    int k, v;
    oml_chk_ok_exit(oml_map_get_iter(&h, &it, &k, &v));
    printf("  (%d, %d)\n", k, v);
  }

  oml_chk_ok_exit(oml_map_cleanup(&h));

  printf("Test successful\n");
  return 0;
}
