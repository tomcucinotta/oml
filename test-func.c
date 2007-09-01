#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_func.h"

oml_func_define(void, f, int x) {
  oml_log_info("x is %d", x);
}

oml_func_define(void, g, int x, int y) {
  oml_log_info("x is %d", x);
  oml_log_info("y is %d", y);
}

int main(int argc, char **argv) {
  f(3);
  f(5);
  g(1, 2);
  g(3, 4);
  return 0;
}
