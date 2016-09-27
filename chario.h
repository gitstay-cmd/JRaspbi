/*
 *  chario.h      header file for Raspberry Pi character I/O functions
 *
 *  The printf functions (sprintf, vprint, printf) are target-agnostic; they
 *  have no awareness of any character output routine.
 *
 *  Traditionally, character output functions are wrapped in the stdout
 *  and stderr streams.  This usually involves writing the necessary newlib
 *  support routines, such as sbrk().
 *
 *  The routines supported by this header allow a program to alter
 *  the character I/O routines used by printf() and others at run-time.
 *
 *  For any routines that would typically use stdout, the program
 *  must invoke chario_OutCharRegister() with a pointer to a suitable
 *  character-output function.  This could be a function that writes
 *  to the video display or to a UART.
 *
 *  Similarly, a program may invoke chario_InCharRegister() with a pointer
 *  to a suitable charcter-input function, to provide incoming characters.
 *  This could be a function that reads from a keyboard or USB port.
 *
 *  You may change the input and output functions as often as you like
 *  by simply calling chario_OutCharRegister() and chario_InCharRegister()
 *  repeatedly.
 *
 *  Each invocation of these routines returns a pointer to the currently
 *  active I/O function.  Your program can save this returned value
 *  for later reuse, to restore the pointer if needed.
 *
 *  Initially, these function pointers are both NULL, which means no
 *  character I/O will occur.  You have to provide a valid function
 *  pointer to turn on character input or output.  Assigning NULL to
 *  either pointer will turn off that operation (input or output).
 */
 

#ifndef  CHARIO_H
#define  CHARIO_H

/*
 *  This header file defines a new putchar() function.  In GCC, putchar
 *  is a macro.  That macro must be #undef to remove it.  Otherwise, the
 *  build will use the macro version of putchar and have all sorts of problems
 *  with the collision.  Ditto for getchar().
 */
#undef  putchar
#undef  getchar



typedef  int			(*incharFuncPtr)(void);
typedef  int			(*outcharFuncPtr)(int);
typedef  int			(*availcharFuncPtr)(void);


/*
 *  chario_OutRegister      register callback function for outchar
 *
 *  The function pointer passed as fp will be called by all character
 *  output functions that need to write to the stdout device.  This includes
 *  printf and putchar.
 *
 *  The function you point to with fp must take a single argument, which
 *  is the char to print passed as an int.  The function must return an
 *  int that is the number of chars output.  This will usually be 1, but
 *  may be a negative value to indicate a failure.
 *
 *  Upon exit, this function returns the previous value of the callback
 *  function pointer.  This lets the calling routine restore the callback
 *  function later, if needed.
 */
outcharFuncPtr				chario_OutRegister(outcharFuncPtr  fp);


/*
 *  chario_InRegister      register callback function for inchar
 *
 *  The function pointer passed as fp will be called by all character
 *  input functions that need to read from the stdin device.  This
 *  includes getchar.
 *
 *  THe function you point to with fp must take a void argument and
 *  return an int.  The returned value will be the char read from
 *  the input device.  The function may return a negative value to
 *  indicate an error condition.
 *
 *  Note that this routine locks.  It will not return until the underlying
 *  hardware or software delivers a character.
 *
 *  Upon exit, this function returns the previous value of the callback
 *  function pointer.  This lets the calling routine restore the callback
 *  function later, if needed.
 */
incharFuncPtr				chario_InRegister(incharFuncPtr  fp);


/*
 *  chario_AvailRegister      register callback function for char check
 *
 *  The function pointer passed as fp will be called by all functions that
 *  check for an available char from the target input device.
 *
 *  The function you point to with fp must take a void argument and
 *  return an int.  The returned value is the number of chars available
 *  from the target device.  Additionally, the function may return a negative
 *  value to indicate an error condition.
 *
 *  Upon exit, this function returns the previous value of the callback
 *  function pointer.  This lets the calling routine restore the callback
 *  function later, if needed.
*/
availcharFuncPtr				chario_AvailRegister(availcharFuncPtr  fp);


/*
 *  putchar      char output function using registered callback function
 *
 *  This function uses the callback function previously registered by a
 *  call to chario_OutRegister() to output the char passed as argument
 *  c to the active output device.
 *
 *  This routine returns the number of chars output.  This is usually 1,
 *  but could be negative if the callback function reports an error.
 */
int								putchar(int  c);



/*
 *  getchar      char input function using registered callback function
 *
 *  This function uses the callback function previously registered by a
 *  call to chario_InRegister() to input a char from the active input
 *  device.
 *
 *  This routine locks until a char is available.  It returns the char read.
 */
int								getchar(void);


/*
 *  availchar     check for available char using registered callback function
 *
 *  This function uses the callback function previously registered by a
 *  call to chario_AvailRegister() to check for available chars on the
 *  active input device.
 *
 *  This routine returns the number of available chars.
 */
int								availchar(void);


#endif

