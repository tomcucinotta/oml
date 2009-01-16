/*
* This file is part of Open Macro Library (OML) - http://oml.sourceforge.net/
*
* Copyright (C) 2008, Alessandro Evangelista <evangelista@gmx.net>
*
* OML is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* OML is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License along with Fluid. If not, see
* <http://www.gnu.org/licenses/>.
*/ 

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
