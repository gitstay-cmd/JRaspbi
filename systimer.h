#ifndef systimer_h_INCLUDED
#define systimer_h_INCLUDED
#include "bcm2835.h"
#include <stdint.h>

#define RPI_SYSTIMER_BASE Peripheral_Base + 0x3000

typedef struct {
    volatile uint32_t control_status;
    volatile uint32_t counter_lo;
    volatile uint32_t counter_hi;
    volatile uint32_t compare0;
    volatile uint32_t compare1;
    volatile uint32_t compare2;
    volatile uint32_t compare3;
} systimer_t;

void waitMicro(uint32_t us);

#endif // systimer_h_INCLUDED

