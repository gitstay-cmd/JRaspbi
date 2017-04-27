#ifndef interrupts_h_INCLUDED
#define interrupts_h_INCLUDED
#include "bcm2835.h"
#include <stdint.h>

#define RPI_INTERRUPT_BASE (Peripheral_Base + 0xB2000)

typedef struct{
    volatile uint32_t IRQ_basic_pending;
    volatile uint32_t IRQ_pending_1;
    volatile uint32_t IRQ_pending_2;
    volatile uint32_t FIQ_control;
    volatile uint32_t Enable_IRQs_1;
    volatile uint32_t Enable_IRQs_2;
    volatile uint32_t Enable_Basic_IRQs;
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
}rpi_irq_controller_t;

#endif // interrupts_h_INCLUDED

