/*
 *  systimer.h      support for free-running 1 MHz system timer
 */

#ifndef  SYSTIMER_H
#define  SYSTIMER_H

/*
 *  THere is nothing to set up for this timer.  Following reset, it starts
 *  automatically, counting up at a 1 MHz rate.  The timer is 64 bits, with
 *  registers for accessing the low and high halves of this 64-bit counter.
 */

#define  SysTimer_GetMicrosecs()	(BCM2835_ST->CLO)
#define  SysTimer_GetMillisecs()	((BCM2835_ST->CLO)/1000)

#endif
