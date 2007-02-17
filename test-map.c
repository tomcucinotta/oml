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

/*   printf("Map dump:\n"); */
/*   while (oml_map_size(&h) > 0) { */
/*     oml_chk_ok_exit(oml_map_get_min(&h, &k, &v)); */
/*     printf("Min map elem: %d, %d\n", k, v); */
/*     oml_chk_ok_exit(oml_map_del_min(&h)); */
/*   } */

/*   int i; */
/*   for (i = 0; i < MAP_SIZE; i++) { */
/*     int rnd = 1 + (int) (100.0 * (rand() / (RAND_MAX + 1.0))); */
/*     oml_chk_ok_exit(oml_map_add(&h, rnd, rnd)); */
/*   } */
/*   if (oml_map_add(&h, 5, 25) != OML_E_FULL) { */
/*     printf("Should have experienced an error on full map\n"); */
/*     return -1; */
/*   } */

/*   for (i = 0; i < MAP_SIZE; i++) */
/*     oml_chk_ok_exit(oml_map_del_min(&h)); */

/*   if (oml_map_del_min(&h) != OML_E_EMPTY) { */
/*     printf("Should have experienced an error on empty map\n"); */
/*     return -1; */
/*   } */

  oml_chk_ok_exit(oml_map_cleanup(&h));

  printf("Test successful\n");
  return 0;
}
