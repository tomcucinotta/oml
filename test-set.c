#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_set.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_set(int);

oml_set(int) h;

#define SET_BITS 3
#define SET_SIZE (1 << SET_BITS)

int main(int argc, char **argv) {
  int v;
  int num_elems;

  oml_chk_ok_exit(oml_set_init(&h, SET_BITS));

  printf("Size of set: %d\n", oml_set_size(&h));
  oml_chk_exit(oml_set_size(&h) == 0);
  oml_chk_ok_exit(oml_set_add(&h, 20));
  printf("Size of set: %d\n", oml_set_size(&h));
  oml_chk_exit(oml_set_size(&h) == 1);

  oml_chk_ok_exit(oml_set_add(&h, 35));
  oml_chk_ok_exit(oml_set_add(&h, 5));
  oml_chk_ok_exit(oml_set_add(&h, 60));
  oml_chk_ok_exit(oml_set_add(&h, 12));
  printf("Size of set: %d\n", oml_set_size(&h));
  oml_chk_exit(oml_set_size(&h) == 5);

  oml_chk_ok_exit(oml_set_add(&h, 12)); // Add again some item
  printf("Size of set: %d\n", oml_set_size(&h));
  oml_chk_exit(oml_set_size(&h) == 5);

  num_elems = 0;
  printf("Set dump: (");
  oml_set_iterator(int) it;
  for (oml_set_begin(&h, &it); oml_set_has_value(&h, &it); oml_set_next(&h, &it)) { 
    oml_set_get(&h, &it, &v);
    printf("%d, ", v);
    num_elems++;
  }
  printf(")\n");
  oml_chk_exit(num_elems == oml_set_size(&h));

  oml_chk_ok_exit(oml_set_del(&h, 60)); // Delete one item
  oml_chk_ok_exit(oml_set_size(&h) == 4);
  oml_chk_ok_exit(oml_set_del(&h, 35)); // Delete one item
  oml_chk_ok_exit(oml_set_del(&h, 12)); // Delete one item
  oml_chk_ok_exit(oml_set_del(&h, 5)); // Delete one item
  oml_chk_exit(oml_set_size(&h) == 0);

/*   int i; */
/*   for (i = 0; i < SET_SIZE; i++) { */
/*     int rnd = 1 + (int) (100.0 * (rand() / (RAND_MAX + 1.0))); */
/*     oml_chk_ok_exit(oml_set_add(&h, rnd, rnd)); */
/*   } */
/*   if (oml_set_add(&h, 5, 25) != OML_E_FULL) { */
/*     printf("Should have experienced an error on full set\n"); */
/*     return -1; */
/*   } */

/*   for (i = 0; i < SET_SIZE; i++) */
/*     oml_chk_ok_exit(oml_set_del_min(&h)); */

/*   if (oml_set_del_min(&h) != OML_E_EMPTY) { */
/*     printf("Should have experienced an error on empty set\n"); */
/*     return -1; */
/*   } */

  oml_chk_ok_exit(oml_set_cleanup(&h));

  printf("Test successful\n");
  return 0;
}
