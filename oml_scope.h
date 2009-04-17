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
#include <stdio.h>

typedef void (*p_oml_destructor)(void *p_data);

typedef struct oml_destr_node {
	p_oml_destructor p_destructor;
	void *p_data;
	struct oml_destr_node *p_next;
} oml_destr_node;

typedef struct __oml_scope_ctx {
	const char * func_name;
	int i;
	oml_destr_node *p_head;
	struct __oml_scope_ctx *p_next;
} oml_scope_ctx;

extern __thread oml_scope_ctx *__p_curr_ctx;

extern oml_scope_ctx __construct(oml_scope_ctx *p_ctx, const char *func_name);
extern void __destroy(oml_scope_ctx *p_ctx);
extern void __return(oml_scope_ctx *p_ctx, const char *func_name);

/** Intercept attempts to return from arbitrary points in scoped sections
 **
 ** @todo
 ** Currently, this introduces a harmless warning on each return attempt from functions returning void.
 **/
//#define return return_is_forbidden__use_oml_return()
//#define return __return(__func__), return 
//#define return return __return(__func__), 

/** Return from scope context without arg */
#define oml_return_void return __return(&__oml_ctx, __func__)

/** Return from scope context with arg */
#define oml_return return __return(&__oml_ctx, __func__), 

/** Start a scoped code segment
 **/
#define oml_scope							\
	printf("__scope: func_name=%s  frame_addr=%p\n",		\
	       __func__, __builtin_frame_address(0));			\
	for (oml_scope_ctx __oml_ctx = __construct(&__oml_ctx, __func__); \
	     __oml_ctx.i < 2;						\
	     ++__oml_ctx.i)						\
		if (__oml_ctx.i == 1) {					\
			__destroy(&__oml_ctx);				\
			__p_curr_ctx = __p_curr_ctx->p_next;		\
		} else

/* #define oml_scope_add(_p_node, _f, _f_data)	\ */
/* 	do {						\ */
/* 		(_p_node)->p_next = __oml_ctx.p_head;	\ */
/* 		(_p_node)->p_destructor = (_f);		\ */
/* 		(_p_node)->p_data = (_f_data);		\ */
/* 		__oml_ctx.p_head = (_p_node);		\ */
/* 	} while (0) */

void __oml_scope_add(oml_scope_ctx *p_ctx, p_oml_destructor p_f, void *p_data);

#define oml_scope_add(_f, _f_data) __oml_scope_add(__p_curr_ctx, (_f), (_f_data))

#endif /* __OML_SCOPE_H__ */
