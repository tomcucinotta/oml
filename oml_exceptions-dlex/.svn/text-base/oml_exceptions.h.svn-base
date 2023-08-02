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

/**
 *  @file		oml_exceptions.h
 *  @brief		This is the main include file for exception handling.
 *  @author		Alessandro Evangelista, Tommaso Cucinotta, Dario Faggioli
 *  @date		1998-2009
 *  @warning		Read carefully the documentation
 */

#ifndef __OML_EXCEPTION
#define __OML_EXCEPTION

#define _GNU_SOURCE        /* or _BSD_SOURCE or _SVID_SOURCE */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

#include <setjmp.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

/**
 * This is the type of any exception variable.
 *
 * @see oml_throw
 */
typedef struct oml_exception_struct oml_exception_struct;

/**
 * This functions prints on stdout the name of the raised exception.
 *
 * @deprecated { this is description why }
 * @see oml_throw
 */
void oml_print_exception();

/**
 * This is the kernel structure of the exception handling package.
 *
 * @see oml_throw
 */
struct oml_exception_struct {
	const char *name;		///< The name of the exception.
	const oml_exception_struct *parent;	///< Pointer to the parent exception structure.
};


/*
	declaration example:

		exception hierarchy:

			ex_file_open, ex_file_close, ex_file_read, ex_file_write -> ex_file -> oml_exception

		file.h contains:

			oml_declare_exception(ex_file);
			oml_declare_exception(ex_file_open);
			oml_declare_exception(ex_file_close);
			oml_declare_exception(ex_file_read);
			oml_declare_exception(ex_file_write);

		file.c contains:

			oml_define_exception( ex_file ) oml_extends( oml_exception );
			oml_define_exception( ex_file_open ) oml_extends( ex_file );
			oml_define_exception( ex_file_close ) oml_extends( ex_file );
			oml_define_exception( ex_file_read ) oml_extends( ex_ile );
			oml_define_exception( ex_file_write ) oml_extends( ex_file );


*/

/**
 * This macro defines a new exception.
 *
 * Use this macro in a source file to define a new exception.
 *
 * For example if you want to extend an exception, let this exception be ex_base, you can write:
 *
 * @code

oml_define_exception( ex_derived ) oml_extends(ex_base);

 * @endcode
 *
 * The exceptions are variables. So, if you want to use an exception in several modules, you
 * have to declare that exception(variable) as extern in the header file of the module that
 * defines the exception.
 *
 * For example in the source file (module.c):
 *
 * @include ex_module.c
 *
 * For example in the header file (module.h):
 *
 * @include ex_module.h
 *
 * @hideinitializer
 *
 * @see oml_extends, oml_declare_exception
 */
#define oml_define_exception(e) \
	const oml_exception_struct e = { #e,

/**
 * This macro declares an exception.
 *
 * Use this macro in an include file to declare an exception.
 *
 * For example if you want to declare the EFileRead, you can write:
 *
 * @code

oml_declare_exception(EFileRead);

 * @endcode
 *
 * @hideinitializer
 *
 * @see oml_define_exception
 */
#define oml_declare_exception(e) \
	extern const oml_exception_struct e

/**
 * This macro is part of a declaration of a new exception.
 *
 * @hideinitializer
 *
 * @see exception
 */
#define oml_extends(e) \
	&e }

/* eccezione base dichiarata in <exception.c> */

/**
 * This is the root exception.
 *
 * Any exception is logically derived from oml_exception exception.
 *
 * @see oml_error, oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */
extern const oml_exception_struct oml_exception;

/**
 * This is the root error exception.
 *
 * Any error exception is logically derived from oml_error exception.
 *
 * oml_error is directly derived from oml_exception.
 *
 * @see oml_exception, oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */
extern const oml_exception_struct oml_error;

/** Generic Time Constraint Violation Exception */
oml_declare_exception(oml_ex_time_constr_violation);
/** Deadline Constraint Violation Exception, extends oml_ex_time_constr_violation */
oml_declare_exception(oml_ex_deadline_violation);
/** WCET Constraint Violation Exception, extends oml_ex_time_constr_violation */
oml_declare_exception(oml_ex_wcet_violation);

/**
 * This is the type of a void function pointer returning void.
 *
 * @see oml_set_terminate
 */
typedef void (*oml_pfv)();

/**
 * This macro defines the maximum number of possible active nested exception handling context
 * at the same time.
 *
 * Change this definition based on your needs.
 *
 * The problem of defining this number is similar to the stack size problem.
 *
 * If you go beyond this limit the next context will be ignored. The oml_handler
 * sensibility is reset after yoy go back to this code point (where you go beyond).
 *
 * @see oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */
#define MAX_NESTED 10

/** Array of exception contexts (from outer try blocks towards inner ones) */
extern __thread jmp_buf __oml_ExcExceptionContext[MAX_NESTED];

/** Index of last occupied context in the above array */
extern __thread int __oml_ExcCurrentContext;

/** Points to the currently thrown exception */
extern __thread const oml_exception_struct* __oml_raised;

/** Aux OML internal variable */
extern __thread int __oml_rv;

/** Distinguishes among the various types of try block
 ** (type internal to OML, not to be used by the end user)
 **/
typedef enum {
	OML_TRY,
	OML_TRY_WITHIN_DL,
	OML_TRY_WITHIN_WCET
} __oml_try_type;

/** Extension to exception context needed for try_within() and
 ** related constructs.
 **/
typedef struct {
	/** @see __OMLTryType */
	__oml_try_type try_type;
	/** Id of POSIX timer used if try_type is one of the time constrainted types */
	timer_t timerid;
	/** Name of the function this context belongs to */
	const char *func_name;
#ifdef OML_WITHIN_BENCH
	 struct timespec __oml_abs_beg, __oml_abs_end;
	 struct timespec __oml_exec_beg, __oml_exec_end;
#endif
} __OMLTimeConstrCtx;

/** Array of additional context info needed for try_within constructs */
extern __thread __OMLTimeConstrCtx __oml_time_constr_ctx[MAX_NESTED];

/** Support function for throwing an exception object (internal to OML) */
extern void __oml_exc_throw(const oml_exception_struct* e);
/** Support function for matching exception objects (internal to OML) */
extern int __oml_exc_match(const oml_exception_struct* e);
/** Support function for try_within_abs construct (internal to OML) */
extern void __oml_exc_within_abs(struct timespec abs_dl_ts);
/** Support function for try_within_rel construct (internal to OML) */
extern void __oml_exc_within_rel(struct timespec rel_dl_ts);
/** Support function for try_within_wcet construct (internal to OML) */
extern void __oml_exc_within_wcet(struct timespec wcet_ts);

/**
 * This functions sets the terminate oml_handler.
 *
 * when the runtime system does not find a corresponding handler for the raised exception,
 * a terminate handler is called.
 *
 * @code

void oml_terminate()

 * @endcode
 *
 * The default terminate handler is the abort function, that returns the program to the operating
 * system.
 *
 * @code

void abort()

 * @endcode
 *
 * The reason of a terminate function call can be a programming error (you did not provide an oml_handler
 * for a particular execution flux) or you intentionally decided to not oml_handle that exception: the only
 * way to treat that condition is to exit the program.
 *
 * @include ex_set_terminate.c
 *
 * @param p_terminate Function pointer to the new terminate oml_handler.
 *
 * @return The old terminate oml_handler.
 *
 * @see oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */
extern oml_pfv oml_set_terminate(oml_pfv p_terminate);


/**
 * This macro starts a oml_try block.
 *
 * In order to construct an exception oml_handler you have to enclose the statements
 * that might oml_throw an exception within a oml_try block.
 *
 * This code fragment shows the a basic idea of how the exception package works:
 *
 * @include ex_overview.c
 *
 * You cannot use the goto statemant going in or going out a oml_try block.
 *
 * For example:
 *
 * @include ex_goto.c
 *
 * The variables of the classes
 * @code

auto

 * @endcode

 * and
 * @code

register

 * @endcode
 * modified in a oml_try block and referenced in the oml_finally block or in one of the oml_when blocks
 * have to be qualified as
 * @code

volatile

 * @endcode
 *
 * For example:
 *
 * @include ex_volatile.c
 *
 * @hideinitializer
 *
 * @see oml_throw, oml_finally, oml_handle, oml_when, oml_end
 *
 * @todo Switch to the do { ... } while (0) paradigm, please !
 */
#define oml_try								\
	__oml_raised = NULL;						\
	oml_try_within_disable();					\
	++__oml_ExcCurrentContext;					\
	__oml_time_constr_ctx[__oml_ExcCurrentContext].try_type = OML_TRY; \
	__oml_time_constr_ctx[__oml_ExcCurrentContext].func_name = __func__; \
	__oml_rv = sigsetjmp (__oml_ExcExceptionContext[__oml_ExcCurrentContext], 1); \
	oml_try_within_enable();					\
	if ( __oml_rv == 0 ) {

#ifndef OML_DEADLINE_SIGNAL
/** Default signal used for a deadline miss notification.
 **
 ** It may be configured by defining OML_DEADLINE_SIGNAL
 ** to a custom signal number before including this file.
 **/
#  define OML_DEADLINE_SIGNAL	SIGRTMIN+1
#endif

#ifndef OML_WCET_SIGNAL
/** Default signal used for a WCET overrun notification.
 **
 ** It may be configured by defining OML_WCET_SIGNAL to
 ** a custom signal number before including this file.
 **/
#  define OML_WCET_SIGNAL		SIGRTMIN+2
#endif

/** Disable asynchronous notification of timing constraint violations to the current thread */
extern void oml_try_within_disable();

/** Enable asynchronous notification of timing constraint violations to the current thread */
extern void oml_try_within_enable();

#ifdef OML_WITHIN_BENCH
#  define __oml_abs_beg __oml_time_constr_ctx[__oml_ExcCurrentContext].__oml_abs_beg
#  define __oml_abs_end __oml_time_constr_ctx[__oml_ExcCurrentContext].__oml_abs_end
#  define __oml_exec_beg __oml_time_constr_ctx[__oml_ExcCurrentContext].__oml_exec_beg
#  define __oml_exec_end __oml_time_constr_ctx[__oml_ExcCurrentContext].__oml_exec_end
#  include <stdio.h>
/** Mark the begin of a benchmarking block (macro internal to OML). */
#  define __oml_bench_begin() \
	 clock_gettime(CLOCK_MONOTONIC, &__oml_abs_beg); \
	 clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__oml_exec_beg)
/** Mark the end of a benchmarking block (macro internal to OML). */
#  define __oml_bench_end() \
	 clock_gettime(CLOCK_MONOTONIC, &__oml_abs_end); \
	 clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__oml_exec_end); \
	 fprintf(stderr, "Elapsed abs in %s:%d : %ld\n", __FILE__, __LINE__, \
		(__oml_abs_end.tv_sec - __oml_abs_beg.tv_sec) * 1000000l \
	    + (__oml_abs_end.tv_nsec - __oml_abs_beg.tv_nsec) / 1000l \
	 ); \
	 fprintf(stderr, "Elapsed exec in %s:%d: %ld\n", __FILE__, __LINE__, \
			   (__oml_exec_end.tv_sec - __oml_exec_beg.tv_sec) * 1000000l \
			   + (__oml_exec_end.tv_nsec - __oml_exec_beg.tv_nsec) / 1000l \
	 )
#else
/** Mark the begin of a benchmarking block (macro internal to OML). */
#  define __oml_bench_begin()
/** Mark the end of a benchmarking block (macro internal to OML). */
#  define __oml_bench_end()
#endif

/** Start an abs-deadline-constrained block
 **
 ** If the special macro OML_WITHIN_BENCH is set (before
 ** including this file), then
 ** all try_within constructs automatically perform a
 ** simple measurement of both the absolute time and the
 ** thread-specific execution time that is needed by the
 ** enclosed code segment in order to complete execution.
 **/
#define oml_try_within_abs(dl_ts) /* struct timespec */ \
	oml_try \
	__oml_exc_within_abs(dl_ts); 

/** Start a rel-deadline-constrained block */
#define oml_try_within_rel(dl_ts) /* struct timespec */ \
	oml_try \
	__oml_exc_within_rel(dl_ts); 

/**
 * This macro starts a oml_finally block.
 *
 * The runtime system always executes the statements within the oml_finally block
 * regardless of what happens within the oml_try block.
 *
 * The oml_finally block let you provide a mechanism for cleaning up the state of
 * the method before allowing control to be passed to a different
 * part of the program without duplicate the code in all oml_handlers.
 *
 * The oml_finally block is optional in a exception handling context.
 *
 * For example:
 *
 * @include ex_nofinally.c
 *
 * Using the oml_finally block:
 *
 * @include ex_finally.c
 *
 * @hideinitializer
 *
 * @see oml_try, oml_throw, oml_handle, oml_when, oml_end
 */
#define oml_finally \
	}{

/**
 * This macro starts a oml_when statements sequence
 *
 * You associate exception oml_handlers with a oml_try statement
 * by providing one or more oml_when blocks after the oml_try oml_handle statement.
 *
 * The oml_handle statement starts a sequence of oml_when statements after the oml_try block.
 *
 * @hideinitializer
 *
 * @see oml_try, oml_throw, oml_when, oml_finally, oml_end
 */
#define oml_handle \
	} \
	if (__oml_time_constr_ctx[__oml_ExcCurrentContext].try_type != OML_TRY) { \
		timer_delete(__oml_time_constr_ctx[__oml_ExcCurrentContext].timerid); \
		__oml_bench_end(); \
	} \
	__oml_ExcCurrentContext--; \
	if (__oml_raised == NULL) {

/**
 * This macro starts a oml_when block.
 *
 * You associate exception oml_handlers with a oml_try statement
 * by providing one or more oml_when blocks after the oml_try oml_handle statement.
 *
 * The oml_handle statement starts a sequence of oml_when statements.
 *
 * The oml_handle block is mandatory in an exception handling context.
 *
 * The minimal form of an exception handling context is:
 *
 * @include ex_minimal.c
 *
 * @hideinitializer
 *
 * @see oml_try, oml_throw, oml_handle, oml_end, oml_finally
 */
#define oml_when(e) \
	} else if ( __oml_exc_match(&e) ) {

/**
 * This macro ends an exception handling code fragment.
 *
 * @hideinitializer
 *
 * @see oml_any, oml_try, oml_throw, oml_handle, oml_finally
 */
#define oml_end \
	} else __oml_exc_throw(__oml_raised);


/**
 * This macro oml_throws an exception.
 *
 * The oml_throw statement requires a single argument: an exception object.
 *
 * @code

throw(oml_exception);	// Raise a generic exception
throw(ex_file_open);	// Raise an app-specific exception

 * @endcode
 *
 * @param e	the exception to oml_throw
 *
 * @hideinitializer
 *
 * @note
 * The exception object must not be a local variable, because its pointer is taken
 * and propagated outside the local context (potentially up to the main())
 *
 * @see oml_rethrow, oml_try, oml_handle
 */
#define oml_throw(e) \
	__oml_exc_throw(&e)

/**
 * This macro specifies 'any exception' as argument of a oml_when statement
 *
 * @deprecated
 * Use simply the oml_exception root type, instead
 *
 * The oml_any argument lets you catch any exception inside a oml_when block.
 *
 * @hideinitializer
 *
 * @see oml_when
 */
#define oml_any	\
	(*__oml_raised)

/**
 * This macro oml_rethrows the current oml_handled exception.
 *
 * You can use the oml_rethrow statement inside a oml_when block to throw again
 * the currently handled exception.
 *
 * @include ex_rethrow.c
 *
 * @hideinitializer
 *
 * @see oml_throw, oml_try, oml_handle, oml_end, oml_finally
 */
#define oml_rethrow \
	__oml_exc_throw(__oml_raised);

/**
 * This macro returns the function
 *
 * You can use the oml_ex_return statement inside a oml_try block to return from
 * the current function.
 *
 * Do not use the standard return keyword inside a oml_try block.
 *
 * @include ex_return.c
 *
 * @hideinitializer
 *
 * @see oml_when, oml_try, oml_handle, oml_end, oml_finally
 *
 * @todo
 * This is assuming that there is exactly one try block wrapping the oml_ex_return call,
 * but actually there might be more than one. Fix by detecting at run-time, within the
 * try macro, the __func function name as soon as it changes and registering the
 * corresponding current context. Such value should be assigned here to the current
 * context, instead of the current simple decrement.
 */
#define oml_ex_return(x)						\
	do {								\
		assert(__oml_ExcCurrentContext >= 0 && __oml_time_constr_ctx[__oml_ExcCurrentContext].func_name == __func__); \
		while (__oml_ExcCurrentContext >= 0 && __oml_time_constr_ctx[__oml_ExcCurrentContext].func_name == __func__) { \
			if (__oml_time_constr_ctx[__oml_ExcCurrentContext].try_type != OML_TRY) { \
				timer_delete(__oml_time_constr_ctx[__oml_ExcCurrentContext].timerid); \
				__oml_bench_end();			\
			}						\
			__oml_ExcCurrentContext--;			\
		}							\
		return (x);						\
	} while (0)

#endif /* __OML_EXCEPTION */
