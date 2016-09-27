
#include "rpi-interrupts.h"
#ifndef ARM_TIMER_H
#define ARM_TIMER_H
#include <stdint.h>
#include "rpi-gpio.h"
#include "rpi-base.h"

#define RPI_ARMTIMER_BASE				( PERIPHERAL_BASE + 0xB400 )

#define RPI_ARMTIMER_CTRL_23BIT			(1 << 1 )

#define RPI_ARMTIMER_CTRL_PRESCALE_1	(0 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_16	(1 << 2 )
#define RPI_ARMTIMER_CTRL_PRESCALE_256	(2 << 2 )

#define RPI_ARMTIMER_CTRL_INT_ENABLE	(1 << 5 )
#define RPI_ARMTIMER_CTRL_INT_DISABLE	(0 << 5 )

#define RPI_ARMTIMER_CTRL_ENABLE		(1 << 7 )
#define RPI_ARMTIMER_CTRL_DISABLE		(0 << 7 )

typedef struct {
		/** Countdown value*/
		volatile uint32_t Load;
		
		/** Current timer value*/
		volatile uint32_t Value;

		volatile uint32_t Control;

		volatile uint32_t IRQClear;

		volatile uint32_t RAWIRQ;

		volatile uint32_t MaskedIRQ;

		volatile uint32_t Reload;

		volatile uint32_t PreDivider;

		volatile uint32_t FreeRunningCounter;
} rpi_arm_timer_t;

extern rpi_arm_timer_t* RPI_GetArmTimer(void);
extern void RPI_ArmTimerInit(void);

#endif
