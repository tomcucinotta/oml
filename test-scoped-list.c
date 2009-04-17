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

#include "oml_scoped_list.h"

int main() {
  oml_list(int) l;
  oml_scope {
    oml_list_scoped_init(&l);
  }
  return 0;
}
