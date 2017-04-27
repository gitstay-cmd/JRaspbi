#include "systimer.h"
#include <stdint.h>

static systimer_t* rpiSystimer = (systimer_t*)RPI_SYSTIMER_BASE;

systimer_t* RPI_GetSystemTimer(void){
    return rpiSystimer;
}

void waitMicro(uint32_t us){
    volatile uint32_t ts = rpiSystimer->counter_lo;

    while((rpiSystimer->counter_lo - ts) < us){
    }
}
