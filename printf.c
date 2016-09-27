/*
 * printf.c
 *
 *  Created on: 18 jul. 2015
 *      Author: Arjan van Vught
 *
 *  Modified 21 Aug 2015  Karl Lunt
 *  Recoded to use common char I/O routines putchar(), getchar(),
 *  and avail().  These routines provide functionality similar to
 *  the newlib standard I/O streams but don't pull in malloc(),
 *  sbrk(), or others.  See the code in chario.c and chario.h for
 *  details.
 *
 *  Added get_line() and get_line_r() to support keyboard entry.
 *  Both routines handle length-delimited input and backspacing.
 *  get_line() blocks until CR, get_line_r() allows background tasks
 *  to run while waiting for character input.
 */

/* Copyright (C) 2015 by Arjan van Vught <pm @ http://www.raspberrypi.org/forum/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

#include "chario.h"
#include "printf.h"




struct context
{
	int flag;
	int prec;
	int width;
	int total;
};

enum {
	FLAG_PRECISION	    =	(1 << 0 ),
	FLAG_UPPERCASE	    =	(1 << 1 ),
	FLAG_LONG     	    =	(1 << 2 ),
	FLAG_NEGATIVE	    =	(1 << 3 ),
	FLAG_MIN_WIDTH	    =	(1 << 4 ),
	FLAG_ZERO_PADDED    =	(1 << 5 ),
	FLAG_LEFT_JUSTIFIED =	(1 << 6 )
};





/*
 *  If outptr is not NULL, it points to a memory buffer to be filled by
 *  routines such as sprintf().  If outputr is NULL, characters are to
 *  be written using putchar().  See chario.h for details on setting
 *  up putchar() for character output.
 */
static  char				*outptr = NULL;



inline static void xputch(struct context *ctx, int c)
{
	ctx->total++;

	if (outptr != NULL)
	{
		*outptr++ = (char) c;
		return;
	}

	putchar(c);
}



inline static bool is_digit(char c)
{
	return (c >= (char)'0') && (c <= (char)'9');
}


inline static void format_hex(struct context *ctx, unsigned int arg)
{
	char			buffer[16];
	char			*p = buffer + (sizeof(buffer) / sizeof(buffer[0])) - 1;
	char			*o = p;
	char			alpha;
	char			u;

	if (arg == 0)
	{
		*p = '0';
		p--;
	}
	else
	{
		alpha = ((ctx->flag & FLAG_UPPERCASE) != 0) ? ((char)'A' - (char)10) : ((char)'a' - (char)10);

		while (arg != 0)
		{
			u = (char)arg & (char)0x0F;
			*p = (u < (char)10) ? ((char)'0' + u) : (alpha + u);
			p--;
			arg = arg >> 4;
		}
	}

	if ((ctx->flag & FLAG_PRECISION) != 0)
	{
		while (((o - p) < ctx->prec) && (p > buffer))
		{
			*p-- = '0';
		}
	}

	if ((ctx->flag & FLAG_ZERO_PADDED) != 0)
	{
		while (((o - p) < ctx->width) && (p > buffer))
		{
			*p-- = '0';
		}
	}

	if ((ctx->flag & FLAG_MIN_WIDTH) != 0)
	{
		while (((o - p) < ctx->width) && (p > buffer))
		{
			*p-- = ' ';
		}
	}

	p++;

	while (p < buffer + (sizeof(buffer) / sizeof(buffer[0])))
	{
		xputch(ctx, (int )*p++);
	}
}



inline static void format_int(struct context *ctx, long int arg)
{
	char				buffer[16];
	char				*p = buffer + (sizeof(buffer) / sizeof(buffer[0])) - 1;
	char				*o = p;

	if (arg == 0)
	{
		*p = '0';
		p--;
	}
	else
	{
		while (arg != 0)
		{
			*p = (char)(arg % 10) + (char)'0';
			p--;
			arg = arg / 10;
		}
	}

	if ((ctx->flag & FLAG_PRECISION) != 0)
	{
		while (((o - p) < ctx->prec) && (p > buffer))
		{
			*p-- = '0';
		}
	}

	if ((ctx->flag & FLAG_ZERO_PADDED) != 0)
	{
		while (((o - p) < ctx->width) && (p > buffer))
		{
			*p-- = '0';
		}
	}

	if((ctx->flag & FLAG_NEGATIVE) != 0)
	{
		*p-- = '-';
	}

	if ((ctx->flag & FLAG_MIN_WIDTH) != 0)
	{
		while (((o - p) < ctx->width) && (p > buffer))
		{
			*p-- = ' ';
		}
	}

	p++;

	while (p < buffer + (sizeof(buffer) / sizeof(buffer[0])))
	{
		xputch(ctx, (int )*p++);
	}
}




int vprintf(const char *fmt, va_list va)
{
	struct context ctx;
	long int l;
	int j;
	const char *s;

	ctx.total = 0;

	while (*fmt != (char)0)
	{

		if (*fmt != '%')
		{
			xputch(&ctx, (int )*fmt++);
			continue;
		}

		fmt++;

		ctx.flag = 0;
		ctx.prec = 0;
		ctx.width = 0;

		if (*fmt == '0')
		{
			ctx.flag |= FLAG_ZERO_PADDED;
			fmt++;
		}
		else if (*fmt == '-')
		{
			ctx.flag |= FLAG_LEFT_JUSTIFIED;
			fmt++;
		}

		while (is_digit(*fmt))
		{
			ctx.width = ctx.width * 10 + (int)(*fmt - '0');
			fmt++;
		}

		if (ctx.width != 0)
		{
			ctx.flag |= FLAG_MIN_WIDTH;
		}

		if (*fmt == '.')
		{
			fmt++;
			while (is_digit(*fmt))
			{
				ctx.prec = ctx.prec * 10 + (int)(*fmt - '0');
				fmt++;
			}
			ctx.flag |= FLAG_PRECISION;
		}

		if (*fmt == 'l')
		{
			fmt++;
			ctx.flag |= FLAG_LONG;
		}

		switch (*fmt)
		{
			case 'c':
			xputch(&ctx, va_arg(va, int));
			break;

			case 'd':
			l = ((ctx.flag & FLAG_LONG) != 0) ? va_arg(va, long int) : (long int) va_arg(va, int);
			if (l < 0) {
				ctx.flag |= FLAG_NEGATIVE;
				l = -l;
			}
			format_int(&ctx, l);
			break;

			case 's':
			s = va_arg(va, const char *);
			for (j = 0; s[j] != (char)0; j++)
				;	// strlen

			if ((ctx.flag & FLAG_PRECISION) != 0)
			{
				if (ctx.prec < j)
				{
					j = ctx.prec;
				}
			}

			while ((((ctx.flag & FLAG_LEFT_JUSTIFIED) ==0 )) && (j++ < ctx.width))
			{
				xputch(&ctx, (int)' ');
			}

			while ((((ctx.flag & FLAG_PRECISION) == 0) || (ctx.prec != 0)) && (*s != (char)0))
			{
				xputch(&ctx, (int )*s++);
				ctx.prec--;
			}

			while (j++ < ctx.width)
			{
				xputch(&ctx, (int)' ');
			}
			break;

			case 'X':
			ctx.flag |= FLAG_UPPERCASE;
			/*@fallthrough@*/
			/* no break */
			case 'x':
			format_hex(&ctx, va_arg(va, unsigned int));
			break;

			default:
			xputch(&ctx, (int)*fmt);
			continue;
			//break;
		}

		fmt++;
	}

	return ctx.total;
}



int printf(const char* fmt, ...)
{
	int i;
	va_list arp;

	outptr = NULL;				// switch off writing to a buffer, switch on console
	va_start(arp, fmt);

	i = vprintf(fmt, arp);

	va_end(arp);

	return i;
}



int sprintf(char *buffer, const char *fmt, ...)
{
	int i;
	va_list arp;

	outptr = buffer;			// switch off writing to console, switch on buffer
	va_start(arp, fmt);

	i = vprintf(fmt, arp);

	va_end(arp);

	*outptr = (char)0;
	outptr = NULL;

	return i;
}


/*
 *  get_line      accumlates chars into a buffer until \r (blocking)
 *
 *  This routine gathers characters using getchar() into the buffer
 *  pointed to by buff until a '\r' is reached.  This routine will
 *  never write more than len chars to the buffer.
 *
 *  Upon exit, the chars will have been echoed using putchar(), the
 *  '\r' char will have been replaced with a null, and the string in
 *  buff will be available to the calling routine.
 *
 *  This routine blocks until getchar() provides a '\r'.
 */
void  get_line (char *buff, int len)
{
	int							c;
	int							idx;
	
	idx = 0;

	for (;;)
	{
		c = getchar();
		if (c == '\r') break;
		if ((c == '\b') && idx)
		{
			idx--;
			putchar(c);
			putchar(' ');				// added by mthomas for Eclipse Terminal plug-in
			putchar(c);
		}
		if ((c >= ' ') && (idx < len - 1))
		{
			buff[idx++] = c; putchar(c);
		}
	}
	buff[idx] = 0;
	putchar('\n');
}



/*
 *  get_line_r      accumulate chars into a buffer until \r
 *
 *  This routine will gather characters from getchar() into a buffer
 *  (pointed to by argument buff).  At each invocation, this
 *  routine gathers all the chars available, up to a \r or a max
 *  of len chars, then returns a value that is 0 if the last char
 *  was not a \r or 1 if it was.
 *
 *  If this routine detects a \r from getchar(), it discards that
 *  char and inserts a null into the string to mark the end; it
 *  also echoes a \n to stdout.  It then returns a value of 1 to
 *  show the string is complete.
 *
 *  If this routine detects a backspace ('\b') and there is
 *  at least one char already in the buffer, this routine will
 *  delete that char and decrement the buffer pointer.  This
 *  routine will also send a sequence of \b-space-\b to stdout,
 *  causing a terminal to remove the most recently typed char.
 *
 *  If this routine detects a printable char and there is room
 *  in the buffer, that char is echoed to stdout and stored in
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
int  get_line_r (char *buff, int len, int* idx)
{
	int						c;
	int						retval;
	int						myidx;

	retval = 0;						// assume no end yet
	myidx = *idx;					// get the working index into the buffer

	if (len < 1)  return  0;		// ignore edge case and outright errors

	if (availchar() && (myidx < len))	// if a char is available and there is room...
	{
		c = getchar();				// get next char
		if (c == '\r')				// if end-of-line...
		{
			buff[myidx] = 0;		// end the string
			putchar('\n');			// echo a newline
			retval = 1;				// show we're done
		}
		else						// not end-of-line, how about backspace...
		{
			if ((c == '\b') && myidx)	// if backspace and at least 1 char in buffer...
			{
				myidx--;			// back up one char in buffer
				putchar(c);			// send the backspace
				putchar(' ');		// make it pretty
				putchar(c);			// now reposition cursor
			}
			if ((c >= ' ') && (myidx < len-1))	// if printable and still room for end-of-line...
			{
				buff[myidx++] = c;	// save char, bump index
				putchar(c);			// and echo the char
			}
		}
		*idx = myidx;				// all done, save updated pointer
	}

	return retval;
}

