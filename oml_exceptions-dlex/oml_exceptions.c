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
 *  \file		oml_exceptions.c
 *  \brief		This is the main include file for exception handling.
 *  \author		Alessandro Evangelista, Tommaso Cucinotta, Dario Faggioli
 *  \date		1998-2009
 *  \warning		Read carefully the documentation
 *
 * Package for exception handling for the C language.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "oml_exceptions.h"

#define __PRNERRNO

#ifdef __PRNERRNO
#include <errno.h>

static void prnerr()
{
#ifdef _AT_DEBUG_
  fprintf(stderr, "[error %d: %s]\n", errno, _sys_errlist[errno]);
#endif
}

#endif


__thread int __oml_ExcCurrentContext = -1;

const oml_exception_struct oml_exception = { "oml_exception", NULL };

__thread int __oml_rv;

oml_define_exception(oml_error) oml_extends(oml_exception);
oml_define_exception(oml_ex_time_constr_violation) oml_extends (oml_exception);
oml_define_exception(oml_ex_deadline_violation) oml_extends (oml_ex_time_constr_violation);
oml_define_exception(oml_ex_wcet_violation) oml_extends (oml_ex_time_constr_violation);

__thread const oml_exception_struct* __oml_raised;
__thread int __oml_raised_target_ctx = 0;

__thread jmp_buf __oml_ExcExceptionContext[MAX_NESTED];
__thread __OMLTimeConstrCtx __oml_time_constr_ctx[MAX_NESTED];

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
static oml_pfv __oml_term_handler = &__oml_exc_abort;

oml_pfv oml_set_terminate(oml_pfv new_handler)
{
	oml_pfv old_handler = __oml_term_handler;
	__oml_term_handler = new_handler;
	return old_handler;
}

static void __oml_terminate()
{
	puts("Terminate()");
	(*__oml_term_handler)();
}

/** This value is returned to the caller of setjmp() or sigsetjmp() when coming
 ** back from an exception throw (or signal fire).
 **
 ** @note
 ** Please, do not use -1 because it is the standard error return code for setjmp()
 **/
#define EXCEPTION_RAISED		1

void __oml_exc_throw(const oml_exception_struct *raising)
{
	if ( __oml_ExcCurrentContext < 0 )
		__oml_terminate();
	/*
	 * se si cerca di sollevare una eccezione quando un'altra ancora
	 * non e' stata gestita si ignora la nuova e si rilancia la vecchia
	 *
	 */
	if ( __oml_raised == NULL)
		__oml_raised = raising;

/* 	oml_print_exception(); */
	siglongjmp(__oml_ExcExceptionContext[__oml_ExcCurrentContext], EXCEPTION_RAISED);
}


#define MATCH		1
#define NOT_MATCH	0

int __oml_exc_match(const oml_exception_struct* e)
{
	const oml_exception_struct* p;

	p = __oml_raised;

/* 	fprintf(stderr, "Checking match with %s\n", e->name); */
	do
	{
		if (p == e) {
/* 			fprintf(stderr, "Checking ctx %d (target %d)\n", __oml_ExcCurrentContext+1, __oml_raised_target_ctx); */
/* 			fprintf(stderr, "conditions: %d, %d\n", (__oml_raised != &oml_ex_deadline_violation && __oml_raised != &oml_ex_wcet_violation), __oml_ExcCurrentContext+1 <= __oml_raised_target_ctx); */
			if ((__oml_raised != &oml_ex_deadline_violation && __oml_raised != &oml_ex_wcet_violation)
			    || __oml_ExcCurrentContext+1 <= __oml_raised_target_ctx) {
				__oml_raised = NULL;
				return MATCH;
			} else {
/* 				fprintf(stderr, "Discarding ctx: %d\n", __oml_ExcCurrentContext+1); */
				return NOT_MATCH;
			}
		}
		/* si risale la gerarchia */
		p = p->parent;
	}
	while (p != NULL);

		/* se p == NULL -> p era EException */

	return NOT_MATCH;
}

static void __oml_deadline_sighand(int sig, siginfo_t *p_siginfo, void *p_uctx)
{
	struct timespec ts_now;

	/*
	 * @TODO: for OML integration this has to be removed,
	 * it is just overhead!
	 */
	if (sig != OML_DEADLINE_SIGNAL) {
		fprintf(stderr, "Wrong signal received by deadline signal handler!\n");
		exit(-1);
	}

	/*
	 * @TODO: for OML integration this has to be removed,
	 * or only compiled at high debugging level.
	 */
	clock_gettime(CLOCK_MONOTONIC, &ts_now);
/* 	pid_t tid = syscall(SYS_gettid); */
/* 	fprintf(stderr, "\tTime Constraint Violation in task %d at time (%lu.%09lu)!\n", (int)tid, ts_now.tv_sec, ts_now.tv_nsec); */

	if ( __oml_ExcCurrentContext < 0 )
		__oml_terminate();
	if ( __oml_raised == NULL) {
		__oml_raised = &oml_ex_deadline_violation;
		__oml_raised_target_ctx = (int) (long unsigned) p_siginfo->si_value.sival_ptr;
	}

/* 	oml_print_exception(); */

	siglongjmp(__oml_ExcExceptionContext[__oml_ExcCurrentContext], EXCEPTION_RAISED);
}

static void __oml_wcet_sighand (int sig, siginfo_t *p_siginfo, void *p_uctx)
{
	struct timespec ts_now;

	/*
	 * @TODO: for OML integration this has to be removed,
	 * it is just overhead!
	 */
	if (sig != OML_DEADLINE_SIGNAL && sig != OML_WCET_SIGNAL) {
	  fprintf(stderr, "Wrong signal received by deadline/wcet signal handler!\n");
		exit(-1);
	}

	/*
	 * @TODO: for OML integration this has to be removed,
	 * or only compiled at high debugging level.
	 */
	clock_gettime(CLOCK_MONOTONIC, &ts_now);
/* 	pid_t tid = syscall(SYS_gettid); */
/* 	printf("\tTime Constraint Violation in task %d at time (%lu.%09lu)!\n", (int)tid, ts_now.tv_sec, ts_now.tv_nsec); */

	if ( __oml_ExcCurrentContext < 0 )
		__oml_terminate();
	if ( __oml_raised == NULL) {
		__oml_raised = &oml_ex_deadline_violation;
		__oml_raised_target_ctx = (int) (long unsigned) p_siginfo->si_value.sival_ptr;
	}

/* 	oml_print_exception(); */

	siglongjmp(__oml_ExcExceptionContext[__oml_ExcCurrentContext], EXCEPTION_RAISED);
}

void __oml_exc_within_abs(struct timespec abs_dl_ts) {
	struct sigevent sigev;
	struct itimerspec tspec;
	pid_t tid = syscall(SYS_gettid);
	struct sigaction sa = {
		.sa_sigaction = &__oml_deadline_sighand,
		.sa_flags = SA_SIGINFO,	/**< Other flags ? */
	};
	sigemptyset(&sa.sa_mask);
	sigaction(OML_DEADLINE_SIGNAL, &sa, NULL);
	sigev.sigev_value.sival_ptr = (void *) (unsigned long) __oml_ExcCurrentContext;
	sigev.sigev_notify = SIGEV_THREAD_ID | SIGEV_SIGNAL;
	sigev.sigev_signo  = OML_DEADLINE_SIGNAL;
	sigev._sigev_un._tid = tid;
	oml_try_within_disable();
	__oml_time_constr_ctx[__oml_ExcCurrentContext].try_type = OML_TRY_WITHIN_DL;
	timer_create(CLOCK_MONOTONIC, &sigev, &__oml_time_constr_ctx[__oml_ExcCurrentContext].timerid);
	oml_try_within_enable();
	__oml_bench_begin();
	tspec.it_interval.tv_sec = tspec.it_interval.tv_nsec = 0;
	tspec.it_value = abs_dl_ts;
	timer_settime(__oml_time_constr_ctx[__oml_ExcCurrentContext].timerid, TIMER_ABSTIME, &tspec, NULL);
}

void __oml_exc_within_rel(struct timespec rel_dl_ts) {
	struct timespec abs_dl_ts;
	clock_gettime(CLOCK_MONOTONIC, &abs_dl_ts);
	abs_dl_ts.tv_sec += rel_dl_ts.tv_sec;
	abs_dl_ts.tv_nsec += rel_dl_ts.tv_nsec;
	if (abs_dl_ts.tv_nsec >= 1000000000ul) {
		abs_dl_ts.tv_nsec -= 1000000000ul;
		abs_dl_ts.tv_sec++;
	}
	__oml_exc_within_abs(abs_dl_ts);
}

void __oml_exc_within_wcet(struct timespec wcet_ts) {
	struct timespec abs_dl_ts;
	struct sigevent sigev;
	struct itimerspec tspec;
	pid_t tid = syscall(SYS_gettid);
	struct sigaction sa = {
		.sa_sigaction = &__oml_wcet_sighand,
		.sa_flags = SA_SIGINFO,	/**< Other flags ? */
	};

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &abs_dl_ts);
	abs_dl_ts.tv_sec += wcet_ts.tv_sec;
	abs_dl_ts.tv_nsec += wcet_ts.tv_nsec;
	if (abs_dl_ts.tv_nsec >= 1000000000ul) {
		abs_dl_ts.tv_nsec -= 1000000000ul;
		abs_dl_ts.tv_sec++;
	}

	sigemptyset(&sa.sa_mask);
	sigaction(OML_DEADLINE_SIGNAL, &sa, NULL);
	sigev.sigev_notify = SIGEV_THREAD_ID | SIGEV_SIGNAL;
	sigev.sigev_signo  = OML_WCET_SIGNAL;
	sigev._sigev_un._tid = tid;
	oml_try_within_enable();
	__oml_time_constr_ctx[__oml_ExcCurrentContext].try_type = OML_TRY_WITHIN_WCET;
	timer_create(CLOCK_THREAD_CPUTIME_ID, &sigev, &__oml_time_constr_ctx[__oml_ExcCurrentContext].timerid);
	oml_try_within_disable();
	__oml_bench_begin();
	tspec.it_interval.tv_sec = tspec.it_interval.tv_nsec = 0;
	tspec.it_value = abs_dl_ts;
	timer_settime(__oml_time_constr_ctx[__oml_ExcCurrentContext].timerid, TIMER_ABSTIME, &tspec, NULL);
}

void oml_try_within_disable() {
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, OML_DEADLINE_SIGNAL);
	sigaddset(&sigset, OML_WCET_SIGNAL);
	pthread_sigmask(SIG_BLOCK, &sigset, NULL);
}

void oml_try_within_enable() {
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, OML_DEADLINE_SIGNAL);
	sigaddset(&sigset, OML_WCET_SIGNAL);
	pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
}
