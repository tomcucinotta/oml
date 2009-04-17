/* This file is part of Open Macro Library (OML) - http://oml.sourceforge.net/
 *
 * Copyright (C) 2008, Tommaso Cucinotta and Alessandro Evangelista
 *
 * OML is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * OML is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with OML. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include "oml_scope.h"

void g(void *p_data) {
  printf("Destructor: %lu\n", (long unsigned) p_data);
}

void f() {
  oml_scope {
    printf("Adding destroyer 1\n");
    oml_scope_add(&g, (void *) 1);
    oml_scope {
      printf("Adding destroyer 2\n");
      oml_scope_add(&g, (void *) 2);
      oml_return_void ;
    }
  }
}

int main() {
  int x = 0;

  oml_scope {
    printf("Adding destroyer: %d\n", 10);
    oml_scope_add(&g, (void *) 10);
    x = x + 1;
  }

  f();

  oml_scope {
    printf("Adding destroyer: %d\n", 11);
    oml_scope_add(&g, (void *) 11);
    x = x + 1;
    if (x == 2)
      oml_return 0;
  }

  return 0;
}
