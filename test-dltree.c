#define OML_DEBUG_LEVEL OML_LEVEL_DEBUG
#include "oml_debug.h"
#include "oml_dltree.h"

#include <stdio.h>
#include <stdlib.h>

oml_define_dltree(int);

oml_dltree(int) h;

void dump(oml_dltree_iterator(int) p) {
  printf("p=%p\n", p);
  if (p != NULL) {
    printf("value : %d\n", p->value);
    printf("parent: %p\n", p->p_parent);
    printf("left  : %p\n", p->p_left_child);
    printf("right : %p\n", p->p_right_child);
    dump(p->p_left_child);
    dump(p->p_right_child);
  }
}

void test_iterator() {
  oml_dltree(int) l;
  oml_dltree_iterator(int) it;
  int i, v;

  oml_chk_ok_exit(oml_dltree_init(&l));
  for (i = 0; i < 3; ++i) {
    oml_chk_ok_exit(oml_dltree_push_left(&l, l.p_root, i));
    printf("Size of dltree: %d\n", oml_dltree_size(&l));
    oml_chk_exit(oml_dltree_size(&l) == i + 1);
    oml_chk_ok(oml_dltree_get(&l, l.p_root, &v));
    oml_chk_exit(v == i);
  }
  printf("\n");  dump(l.p_root);
  oml_chk_ok_exit(oml_dltree_begin(&l, &it));
  for (i = 0; i < 3; ++i) {
    oml_chk(oml_dltree_has_value(&l, &it));
    oml_chk_ok(oml_dltree_get(&l, it, &v));
    printf("Value: %d, it: %p\n", v, it);
    oml_chk_exit(v == 2 - i);
    oml_dltree_next(&l, &it);
  }
  i = 0;
  for (oml_dltree_begin(&l, &it); oml_dltree_has_value(&l, it); oml_dltree_next(&l, &it)) {
    oml_chk_ok(oml_dltree_get(&l, it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == 2 - i++);
  }
  oml_chk_ok_exit(oml_dltree_cleanup(&l));

  printf("\n");  dump(l.p_root);
  for (i = 0; i < 3; ++i) {
    oml_chk_ok_exit(oml_dltree_push_right(&l, l.p_root, i));
    printf("Size of dltree: %d\n", oml_dltree_size(&l));
    oml_chk_exit(oml_dltree_size(&l) == i + 1);
    oml_chk_ok(oml_dltree_get(&l, l.p_root, &v));
    oml_chk_exit(v == i);
  }
  printf("\n");  dump(l.p_root);
  oml_chk_ok_exit(oml_dltree_begin(&l, &it));
  for (i = 0; i < 3; ++i) {
    oml_chk(oml_dltree_has_value(&l, &it));
    oml_chk_ok(oml_dltree_get(&l, it, &v));
    printf("Value: %d, it: %p\n", v, it);
    oml_chk_exit(v == 2 - i);
    oml_dltree_next(&l, &it);
  }
  i = 0;
  for (oml_dltree_begin(&l, &it); oml_dltree_has_value(&l, it); oml_dltree_next(&l, &it)) {
    oml_chk_ok(oml_dltree_get(&l, it, &v));
    printf("Value: %d\n", v);
    oml_chk_exit(v == 2 - i++);
  }
  oml_chk_ok_exit(oml_dltree_cleanup(&l));
}

int main(int argc, char **argv) {
  int v;

  oml_chk_ok_exit(oml_dltree_init(&h));

  printf("Size of dltree: %d\n", oml_dltree_size(&h));
  oml_chk_exit(oml_dltree_size(&h) == 0);
  printf("\n");  dump(h.p_root);

  oml_chk_ok_exit(oml_dltree_push_left(&h, h.p_root, 1024));
  printf("Size of dltree: %d\n", oml_dltree_size(&h));
  oml_chk_exit(oml_dltree_size(&h) == 1);
  oml_chk_exit(oml_dltree_front(&h, &v) == OML_OK && (v == 1024));
  printf("\n");  dump(h.p_root);

  oml_chk_ok_exit(oml_dltree_clear(&h));
  printf("Size of dltree: %d\n", oml_dltree_size(&h));
  oml_chk_exit(oml_dltree_size(&h) == 0);
  oml_chk_exit(oml_dltree_empty(&h));

  oml_chk_ok_exit(oml_dltree_push_left(&h, h.p_root, 4));
  printf("\n");  dump(h.p_root);
  oml_chk_ok_exit(oml_dltree_push_left(&h, h.p_root, 7));
  printf("\n");  dump(h.p_root);
  oml_chk_ok_exit(oml_dltree_push_left(&h, h.p_root, 1));
  printf("\n");  dump(h.p_root);
  oml_chk_ok_exit(oml_dltree_push_left(&h, h.p_root, 3));
  printf("\n");  dump(h.p_root);
  printf("Size of dltree: %d\n", oml_dltree_size(&h));
  oml_chk_exit(oml_dltree_size(&h) == 4);

  oml_chk_ok_exit(oml_dltree_clear(&h));
  printf("Size of dltree: %d\n", oml_dltree_size(&h));
  oml_chk_exit(oml_dltree_size(&h) == 0);
  oml_chk_exit(oml_dltree_empty(&h));

  if (oml_dltree_front(&h, &v) != OML_E_EMPTY) {
    printf("Should have experienced an error on empty dltree\n");
    return -1;
  }

  test_iterator();

  oml_chk_exit(oml_mem_clean());
  printf("Test successful\n");
  return 0;
}
