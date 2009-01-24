#include <oml_exceptions/oml_exceptions.h>
#include <stdio.h>
#include <stdlib.h>

oml_define_exception(ENotReady) oml_extends(EException);

void f()
{
  oml_throw(ENotReady);
}

int main()
{
  oml_try
  {
    f();
  }
  oml_handle
      oml_when (EException)
      {
        printf("Exception caught !\n");
      }
  oml_end;

  char *my_mem;
  oml_try
  {
    my_mem = (char*) malloc(100);
    /* here some exception can be raised */
    oml_throw(ENotReady);
  }
  oml_finally
  {
    free(my_mem);
  }
  oml_handle
    oml_when(ENotReady)
    {
      printf("Exception caught !\n");
    }
  oml_end;

  return 0;
}
