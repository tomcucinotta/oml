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

/** 
 *  \file		oml_exceptions.h
 *  \brief		This is the main include file for exception handling.
 *  \author		Alessandro Evangelista
 *  \version		0.9
 *  \date		1998-2009
 *  \bug		Probably the library is not thread-safe
 *  \warning		Read carefully the documentation
 */

#ifndef __OML_EXCEPTION
#define __OML_EXCEPTION

#include <setjmp.h>


/**
 * This is the type of any exception variable.
 *
 * \see oml_throw
 */ 
typedef struct Exception Exception;

/**
 * This functions prints on stdout the name of the raised exception.
 *
 * \deprecated { this is description why }
 * \see oml_throw
 */ 
void oml_print_exception();

/**
 * This is the kernel structure of the exception handling package.
 *
 * \see oml_throw
 */ 
struct Exception {
	const char	*name;		///< The name of the exception.
	const Exception	*parent;	///< Pointer to the parent exception structure.
};


/* 
	declaration example:

		exception hierarchy:

			EException -> Efile -> EFileOpen, EfileClose, EFileRead, EFileWrite 
		
		file.h contains:
		  
			oml_declare_exception(EFile);
			oml_declare_exception(EFileOpen);
			oml_declare_exception(EFileClose);
			oml_declare_exception(EFileRead);
			oml_declare_exception(EFileWrite);

		file.c contains:

			oml_define_exception( EFile ) oml_extends( EException );
			oml_define_exception( EFileOpen ) oml_extends( EFile );
			oml_define_exception( EFileClose ) oml_extends( EFile );
			oml_define_exception( EFileRead ) oml_extends( EFile );
			oml_define_exception( EFileWrite ) oml_extends( EFile );

	
*/

/**
 * This macro defines a new exception.
 *
 * Use this macro in a source file to define a new exception.
 *
 * For example if you want to extend an exception, let this exception EBase, you can write:
 *
 * \code

oml_define_exception( EDerived ) oml_extends(EBase);

 * \endcode
 *
 * The exceptions are variables. So, if you want to use an exception in several modules, you 
 * have to declare that exception(variable) as extern in the header file of the module that 
 * defines the exception.
 *
 * For example in the source file (module.c):
 *
 * \include ex_module.c
 *
 * For example in the header file (module.h):
 *
 * \include ex_module.h
 *
 * \hideinitializer
 *
 * \see oml_extends, oml_declare_exception
 */ 
#define oml_define_exception(e) \
	const Exception e = { #e, 

/**
 * This macro declares an exception.
 *
 * Use this macro in an include file to declare an exception.
 *
 * For example if you want to declare the EFileRead, you can write:
 *
 * \code

oml_declare_exception(EFileRead);

 * \endcode
 *
 * \hideinitializer
 *
 * \see oml_define_exception
 */ 
#define oml_declare_exception(e) \
	extern Exception e

/**
 * This macro is part of a declaration of a new exception.
 *
 * \hideinitializer
 *
 * \see exception
 */ 
#define oml_extends(e) \
	&e }

/* eccezione base dichiarata in <exception.c> */

/**
 * This is the root exception.
 *
 * Any exception is logically derived from EException exception.
 *
 * \see EError, oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */ 
extern const Exception EException;

/**
 * This is the root error exception.
 *
 * Any error exception is logically derived from EError exception.
 *
 * EError is directly derived from EException.
 *
 * \see EException, oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */
extern const Exception EError;


/**
 * This is the type of a void function pointer returning void.
 *
 * \see oml_set_terminate
 */
typedef void (*PFV)();

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
 * \see oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */ 
#define MAX_NESTED 10

extern __thread jmp_buf __oml_ExcExceptionContext[MAX_NESTED];
extern __thread int __oml_ExcCurrentContext;
extern __thread const Exception* __oml_raised;

extern void __oml_exc_throw(const Exception* e);
extern int __oml_exc_match(const Exception* e);

/**
 * This functions sets the terminate oml_handler.
 *
 * oml_when the runtime system does not find a corrisponding oml_handler for the raised exception,
 * a terminate oml_handler is called.
 *
 * \code

void oml_terminate()

 * \endcode	
 *
 * The default terminate oml_handler is the abort function, that return the program to the operating
 * system.
 *
 * \code

void abort()

 * \endcode	
 * 
 * The reason of a terminate function call can be a programming error (you did not provide an oml_handler
 * for a particular execution flux) or you intentionally decided to not oml_handle that exception: the only
 * way to treat that condition is to exit the program.
 *
 * \include ex_set_terminate.c
 *
 * \param pTerminate Function pointer to the new terminate oml_handler.
 *
 * \return The old terminate oml_handler.
 *
 * \see oml_try, oml_throw, oml_finally, oml_handle, oml_when, oml_end
 */ 
extern PFV oml_set_terminate(PFV pTerminate);


/**
 * This macro starts a oml_try-block.
 *
 * In order to construct an exception oml_handler you have to enclose the statements 
 * that might oml_throw an exception within a oml_try block. 
 *
 * This code fragment shows the a basic idea of how the exception package works:
 *
 * \include ex_overview.c
 *
 * You cannot use the goto statemant going in or going out a oml_try-block.
 *
 * For example:
 *
 * \include ex_goto.c
 *
 * The variables of the classes 
 * \code

auto

 * \endcode

 * and	
 * \code

register

 * \endcode
 * modified in a oml_try-block and referenced in the oml_finally-block or in one of the oml_when-blocks
 * have to be qualified as
 * \code

volatile

 * \endcode
 *
 * For example:
 *
 * \include ex_volatile.c 
 *
 * \hideinitializer
 *
 * \see oml_throw, oml_finally, oml_handle, oml_when, oml_end 
 */ 
#define oml_try \
	__oml_raised = NULL; \
	if ( setjmp (__oml_ExcExceptionContext[++__oml_ExcCurrentContext])  == 0 ) {

/**
 * This macro starts a oml_finally-block.
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
 * \include ex_nofinally.c
 *
 * Using the oml_finally block: 
 *
 * \include ex_finally.c
 *
 * \hideinitializer
 * 
 * \see oml_try, oml_throw, oml_handle, oml_when, oml_end
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
 * \hideinitializer
 *
 * \see oml_try, oml_throw, oml_when, oml_finally, oml_end
 */ 
#define oml_handle \
	} __oml_ExcCurrentContext--; if (__oml_raised == NULL) {

/**
 * This macro starts a oml_when-block.
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
 * \include ex_minimal.c
 *
 * \hideinitializer
 *
 * \see oml_try, oml_throw, oml_handle, oml_end, oml_finally
 */ 
#define oml_when(e) \
	} else if ( __oml_exc_match(&e) ) {

/**
 * This macro ends an exception handling code fragment. 
 * 
 * \hideinitializer
 *
 * \see any, oml_try, oml_throw, oml_handle, oml_finally
 */ 
#define oml_end \
	} else __oml_exc_throw(__oml_raised);


/**
 * This macro oml_throws an exception.
 *
 * The oml_throw statement requires a single argument: an Exception object. 
 * 
 * \code

throw(EException);

 * \endcode
 *
 * \param e	the exception to oml_throw
 *
 * \hideinitializer
 *
 * \see oml_rethrow, oml_try, oml_handle
 */ 
#define oml_throw(e) \
	__oml_exc_throw(&e)

/**
 * This macro specify 'any exception' as argument of a oml_when statement
 *
 * The any argument let you catch any exception inside a oml_when-block.
 *
 * \hideinitializer
 *
 * \see oml_when
 */ 
#define oml_any	\
	(*__oml_raised)

/**
 * This macro oml_rethrows the current oml_handled exception.
 *
 * You can use the oml_rethrow statement inside a oml_when-block to oml_throw again
 * the current oml_handled exception.
 * 
 * \include ex_rethrow.c
 *
 * \hideinitializer
 *
 * \see oml_throw, oml_try, oml_handle, oml_end, oml_finally
 */ 
#define oml_rethrow \
	__oml_exc_throw(__oml_raised);

/**
 * This macro returns the function 
 *
 * You can use the oml_ereturn statement inside a oml_try-block to return from
 * the current function.
 *
 * Do not use return inside a oml_try-block.
 *
 * \include ex_return.c
 *
 * \hideinitializer
 *
 * \see oml_when, oml_try, oml_handle, oml_end, oml_finally
 */ 
#define oml_ereturn(x) \
	{ __oml_ExcCurrentContext--; return(x)}

#endif /* __OML_EXCEPTION */
