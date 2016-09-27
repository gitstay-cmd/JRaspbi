/*
 *  chario.c      library to support single-char I/O
 *
 *  These routines provide single-character I/O for the higher
 *  level functions, such as printf().
 *
 *  To uses these routines, you must register a callback
 *  function for character input and a callback function for
 *  character output.  Once these are registered, they will
 *  be invoked by printf() and similar string I/O routines.
 *  This allows you to connect different devices to the string
 *  routines at run-time.
 *
 *  Note that these routines replace the newlib getchar() and
 *  putchar(), which in turn removes the need for sbrk() and
 *  malloc().
 *
 *  Karl Lunt   17 Aug 2015
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#include "chario.h"




outcharFuncPtr				output_func = NULL;

incharFuncPtr				input_func = NULL;

availcharFuncPtr			avail_func = NULL;


outcharFuncPtr  chario_OutRegister(outcharFuncPtr  fp)
{
	outcharFuncPtr			retfp;

	retfp = output_func;
	output_func = fp;
	return  retfp;
}



incharFuncPtr  chario_InRegister(incharFuncPtr  fp)
{
	incharFuncPtr			retfp;

	retfp = input_func;
	input_func = fp;
	return  retfp;
}



availcharFuncPtr  chario_AvailRegister(availcharFuncPtr  fp)
{
	availcharFuncPtr			retfp;

	retfp = avail_func;
	avail_func = fp;
	return  retfp;
}



/*
 *  putchar      standard char output routine using output redirection
 *
 *  This routine is here to replace the standard putchar() that
 *  gets called if you do a single-char printf(), like printf('c').
 */
int  putchar(int  c)
{
	if (output_func)	(*output_func)(c);
	return  1;
}



/*
 *  getchar      standard char input routine using input redirection
 *
 *  This routine is here to replace the standard getchar().
 */
int  getchar(void)
{
	int			c;

	c = 0;
	if (input_func)	 c = (*input_func)();
	return  c;
}



/*
 *  availchar      check char input device for available chars
 *
 *  This routine returns the number of chars available from the
 *  input device.
 */
int  availchar(void)
{
	int					chars;

	chars = 0;
	if (avail_func)  chars = (*avail_func)();
	return  chars;
}
