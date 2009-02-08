#ifndef __OML_SCOPE_H__
#define __OML_SCOPE_H__

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

/** @file
 ** Generic scope construct that aims at allowing for registration of
 ** appropriate destructor(s) with a code segment, and enforcing their call
 ** when the scope is exited.
 **
 **/

#include <assert.h>

typedef struct {
  const char * func_name;
} ScopeCtx, *pScopeCtx;

#define __MAX_NUM_CTXS 16

ScopeCtx ctxs[__MAX_NUM_CTXS];
int ctxs_size = 0;

#define __construct() do {			\
    assert(ctxs_size < __MAX_NUM_CTXS);		\
    ctxs[ctxs_size++].func_name = __func__;	\
  } while (0)

/** Intercept attempts to return from arbitrary points in scoped sections
 **
 ** @todo
 ** Currently, this introduces a harmless warning on each return attempt from functions returning void.
 **/
//#define return return __return(__func__), 
#define return __return(__func__), return 

/** Start a scoped code segment
 **/
#define oml_scope				\
  for (int __i = 0; __i < 3; ++__i)		\
    if (__i == 0) {				\
      __construct();				\
    } else if (__i == 2)			\
      __destroy(__func__);			\
    else

void __destroy(const char *func_name) {
  if ( ctxs_size > 0 && ctxs[ctxs_size - 1].func_name == func_name ) {
    printf("Destroy, him, my robots !\n");
    ctxs_size--;
  }
}

void __return(const char *func_name) {
  __destroy(func_name);
}

#endif /* __OML_SCOPE_H__ */
