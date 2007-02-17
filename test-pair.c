#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_pair.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_pair(int, double);

oml_pair(int, double) h;

int main(int argc, char **argv) {
  oml_chk_ok_exit(oml_pair_init(&h, 3, 4.0));
  oml_chk_exit(oml_pair_first(&h) == 3);
  oml_chk_exit(oml_pair_second(&h) == 4.0);
  oml_pair_first(&h) = 7;
  oml_pair_second(&h) = 7.5;
  oml_chk_exit(oml_pair_first(&h) == 7);
  oml_chk_exit(oml_pair_second(&h) == 7.5);

  printf("Test successful\n");
  return 0;
}
