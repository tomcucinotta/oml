#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_map.h"

#include <stdio.h>
#include <stdlib.h>

typedef char* CHARP;
oml_define_map(CHARP,int);

oml_map(CHARP, int) h;

#define MAP_BITS 3
#define MAP_SIZE (1 << MAP_BITS)

int main(int argc, char **argv) {
  // int->int map
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

  printf("Test successful\n");
  return 0;
}
