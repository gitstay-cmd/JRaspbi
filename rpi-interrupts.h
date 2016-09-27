#ifndef RPI_INTERRUPTS_H
#define RPI_INTERRUPTS_H

#include <stdint.h>

#include "rpi-base.h"
#include "rpi-gpio.h"

#define RPI_BASIC_ARM_TIMER_IRQ			(1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ		(1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ	(1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ	(1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ		(1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ		(1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ	(1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ	(1 << 7)

#define RPI_INTERRUPT_CONTROLLER_BASE (PERIPHERAL_BASE + 0xB200)

typedef struct {
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
} rpi_irq_controller_t;

extern volatile int calculate_frame_count;

extern void _enable_interrupts(void);
extern rpi_irq_controller_t* RPI_GetIrqController(void);
#endif
