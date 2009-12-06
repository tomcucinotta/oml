#ifndef __OML_FOREACH__
#define __OML_FOREACH__

/** Example of usage:
 **
 ** @code
 **   oml_list(int) l;
 **   oml_list_iterator(int) it, it_end;
 **   oml_list_begin(&l, &it);
 **   oml_list_end(&l, &it);
 **   oml_foreach(list, &l, &it, &it_end) {
 **     int value;
 **     oml_list_get(&l, &it, &v);
 **     // Use value
 **   }
 **   @endcode
 **/
#define oml_foreach(type, this, p_it, p_end) \
  for (; ! oml_ ##type## _iter_eq((this), (p_it), (p_end)); oml_ ##type## _next((this), p_it))

#endif /* __OML_FOREACH__ */
