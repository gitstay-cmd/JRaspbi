#include <stdint.h>

#include "timer.h"

void RPI_WaitMicroSeconds(uint32_t us){
	volatile uint32_t ts = rpiSystemTimer->counter_lo;
	while((rpiSystemTimer ->counter_lo -ts) < us){
	}
}

static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;

rpi_sys_timer_t* RPI_GetSystemTimer(void){
	return rpiSystemTimer;
}
