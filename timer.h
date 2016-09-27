#ifndef TIMER
#define TIMER

#include <stdint.h>

#include "rpi-gpio.h"

#ifdef RPI2
	#define RPI_SYSTIMER_BASE		0x3F003000
#else
	#define RPI_SYSTIMER_BASE		0x20003000
#endif

typedef struct {
	volatile uint32_t control_status;
	volatile uint32_t counter_lo;
	volatile uint32_t counter_hi;
	volatile uint32_t compare0;
	volatile uint32_t compare1;
	volatile uint32_t compare2;
	volatile uint32_t compare3;
} rpi_sys_timer_t;

static rpi_sys_timer_t* rpiSystemTimer;
extern rpi_sys_timer_t* RPI_GetSystemTimer(void);
extern void RPI_WaitMicroSeconds( uint32_t us);
#endif
