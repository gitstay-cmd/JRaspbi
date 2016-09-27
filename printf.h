/*
 *  printf.h      header file for Raspberry Pi character stream functions
 *
 *  The printf functions (sprintf, vprint, printf) are target-agnostic; they
 *  have no awareness of any character output routine.
 *
 *  Traditionally, character output functions are wrapped in the stdout
 *  and stderr streams.  This usually involves writing the necessary newlib
 *  support routines, such as sbrk().
 *
 *  This version of the printf routines relies on a library called chario
 *  to provide a way to connect an output device to the printf() routines.
 *  See the chario.h include file for detail on using this mechanism.  In
 *  particular, printf() sends chars to an output device using the putchar()
 *  function.
 *
 *  THis library also includes routines for reading a string from an input
 *  device.  Although reading a stream of chars from an input device isn't
 *  really part of printf, I didn't know where else to put these routines.
 *  These routines read chars using getchar() and echo chars using putchar().
 *  
 */

#ifndef  PRINTF_H
#define  PRINTF_H


/*
 *  get_line      input chars into a buffer until \r (blocking)
 *
 *  This routine reads chars using getchar() until it detects a
 *  '\r'.  Characters are copied to the buffer pointed to by
 *  argument buff (to a max of len chars).  Upon reading a
 *  '\r', the CR is discarded, a NULL is written to the buffer,
 *  and control returns.
 *
 *  This routine processes any backspace chars by deleting the
 *  latest char (if any) and echoing a BS-space-BS sequence with
 *  putchar().
 */
void				get_line (char *buff, int len);



/*
 *  get_line_r      innput chars into a buffer until \r (non-blocking)
 *
 *  This routine will gather characters from getchar() into a buffer
 *  (pointed to by argument buff).  At each invocation, this
 *  routine gathers all the chars available, up to a \r or a max
 *  of len chars, then returns a value that is 0 if the last char
 *  was not a \r or 1 if it was.
 *
 *  If this routine detects a \r from getchar(), it discards that
 *  char and inserts a null into the string to mark the end; it
 *  also echoes a \n to putchar().  It then returns a value of 1 to
 *  show the string is complete.
 *
 *  If this routine detects a backspace ('\b') and there is
 *  at least one char already in the buffer, this routine will
 *  delete that char and decrement the buffer pointer.  This
 *  routine will also send a sequence of \b-space-\b to putchar(),
 *  causing a terminal to remove the most recently typed char.
 *
 *  If this routine detects a printable char and there is room
 *  in the buffer, that char is echoed to putchar() and stored in
 *  the buffer; the index pointed to by idx is incremented.
 *
 *  This routine is intended to be called repeatedly; it does
 *  not block.  This allows the calling routine to perform
 *  background tasks until a user enters a terminating \r.
 *  A typical example would be:
 *
 *    index = 0;
 *    while (get_line_r(buff, 80, &index) == 0)
 *    {
 *      <do some background stuff here>
 *    }
 *    <process input from user>
 *
 */ 
int					get_line_r (char *buff, int len, int* idx);


#endif

