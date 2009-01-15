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


int ExcCurrentContext = -1;

Exception EException = { "EException", NULL };

oml_define_exception(EError) oml_extends(EException);

Exception* __oml_raised;

jmp_buf ExcExceptionContext[MAX_NESTED];

void oml_print_exception() {
  if (__oml_raised!=NULL) {
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

PFV term_handler = &__oml_exc_abort;

PFV oml_set_terminate(PFV new_handler)
{
	PFV old_handler = term_handler;
	term_handler = new_handler;
	return old_handler;
}

static void __oml_terminate()
{
	puts("Terminate()");
	(*term_handler)();
}

#define EXCEPTION_RAISED		-1

void __oml_exc_throw(raising)
Exception *raising;
{
  /**** startdel */
  /* if ( raised == NULL) raised = raising; */
  /*print_exception();*/
  /*  exit(-17); */
  /**** enddel */

  if ( ExcCurrentContext < 0 )
    __oml_terminate();
  /* 
   * se si cerca di sollevare una eccezione quando un'altra ancora
   * non e' stata gestita si ignora la nuova e si rilancia la vecchia
   *
   */
  if ( __oml_raised == NULL)
    __oml_raised = raising;

    oml_print_exception();

  longjmp(ExcExceptionContext[ExcCurrentContext], EXCEPTION_RAISED);
}


#define MATCH		1
#define NOT_MATCH	0

int __oml_exc_match(e)
Exception* e;
{
	Exception* p;
	
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
