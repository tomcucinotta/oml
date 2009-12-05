#include <oml_class.h>
#include "test-myclass.h"

#include <oml_class_serialize.h>
#include "test-myclass.h"

#include <oml_class_define.h>
#include "test-myclass.h"

#include <stdio.h>

unsigned char buf[128];

int main(int argc, char *argv[]) {
  myclass *p_myclass = myclass_new();
  p_myclass->i = 1;
  p_myclass->l = 2;
  p_myclass->d = 3.0;

  unsigned char *tmp_buf = buf;
  unsigned long size = sizeof(buf);
  serialize_myclass(p_myclass, &tmp_buf, &size);

  printf("Written %lu bytes to buffer:\n", tmp_buf - buf);
  for (unsigned char *p = buf; p < tmp_buf; ++p)
    printf("%02X ", *p);
  printf("\n");

  return 0;
}
