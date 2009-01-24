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
* You should have received a copy of the GNU Lesser General Public License along with OML. If not, see
* <http://www.gnu.org/licenses/>.
*/

/*
 * exception.c
 *
 * Package for exception handling for the C language.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "oml_exceptions.h"

#define __PRNERRNO

#ifdef __PRNERRNO
#include <errno.h>

static void prnerr()
{
#ifdef _AT_DEBUG_
  printf("[error %d: %s]\n", errno, _sys_errlist[errno]);
#endif
}

#endif


__thread int __oml_ExcCurrentContext = -1;

const Exception EException = { "EException", NULL };

oml_define_exception(EError) oml_extends(EException);

__thread const Exception* __oml_raised;

__thread jmp_buf __oml_ExcExceptionContext[MAX_NESTED];

void oml_print_exception() {
  if (__oml_raised != NULL) {
    puts(__oml_raised->name);
#ifdef __PRNERRNO
    prnerr();
#endif
  }
}

static void __oml_exc_abort()
{
  oml_print_exception();
  abort();
}

/** This variable is lacking the thread-local qualifier on purpose:
 ** the termination handler should be a singleton, not per-thread.
 **/
static PFV __oml_term_handler = &__oml_exc_abort;

PFV oml_set_terminate(PFV new_handler)
{
	PFV old_handler = __oml_term_handler;
	__oml_term_handler = new_handler;
	return old_handler;
}

static void __oml_terminate()
{
	puts("Terminate()");
	(*__oml_term_handler)();
}

#define EXCEPTION_RAISED		-1

void __oml_exc_throw(const Exception *raising)
{
  /**** startdel */
  /* if ( raised == NULL) raised = raising; */
  /*print_exception();*/
  /*  exit(-17); */
  /**** enddel */

  if ( __oml_ExcCurrentContext < 0 )
    __oml_terminate();
  /*
   * se si cerca di sollevare una eccezione quando un'altra ancora
   * non e' stata gestita si ignora la nuova e si rilancia la vecchia
   *
   */
  if ( __oml_raised == NULL)
    __oml_raised = raising;

    oml_print_exception();

  longjmp(__oml_ExcExceptionContext[__oml_ExcCurrentContext], EXCEPTION_RAISED);
}


#define MATCH		1
#define NOT_MATCH	0

int __oml_exc_match(const Exception* e)
{
	const Exception* p;

	p = __oml_raised;

	do
	{
	  if (p == e) {
	    __oml_raised = NULL;
	    return MATCH;
	  }
		/* si risale la gerarchia */
		p = p->parent;
	}
	while (p != NULL);

		/* se p == NULL -> p era EException */

	return NOT_MATCH;

}
