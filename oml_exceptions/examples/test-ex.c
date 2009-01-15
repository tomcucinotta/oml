#include <oml_exceptions.h>
#include <stdio.h>

oml_define_exception(ENotReady) oml_extends(EException);

void f() {
  oml_throw(ENotReady);
}

int main() {
  oml_try {
    f();
  } oml_handle
      oml_when (EException) {
      printf("Exception caught !\n");
    }
  oml_end
  return 0;
}
