#include<stdint.h>
#include<stdbool.h>
#include "rpi-interrupts.h"
#include "arm-timer.h"
#include "rpi-gpio.h"

static rpi_irq_controller_t* rpiIRQController = (rpi_irq_controller_t*)RPI_INTERRUPT_CONTROLLER_BASE;

volatile int calculate_frame_count = 0;

rpi_irq_controller_t* RPI_GetIrqController(void){
	return rpiIRQController;
}

void __attribute__((interrupt("SWI"))) software_interrupt_vector(void){
	while(1){
		LED_ON();
	}
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void){
	while(1){
		LED_ON();
	}
}

void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void){
	while(1){
		LED_ON();
	}
}

void __attribute__((interrupt("ABORT"))) reset_vector(void){
	while(1){
		LED_ON();
	}
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void){
	while(1){
		LED_ON();
	}
}

void __attribute__((interrupt("ABORT"))) data_abort_vector(void){
	while(1){
		LED_ON();
	}
}

void __attribute__((interrupt("IRQ"))) interrupt_vector(void){
	static int lit = 0;
	static int ticks = 0;
	static int seconds = 0;

	/* Clear Arm Timer interrupt*/
	RPI_GetArmTimer()->IRQClear = 1;
	
	ticks++;
	if(ticks > 1){
		ticks = 0;
		
		/* Calculate the FPS once a minute*/
		seconds++;
		if(seconds > 59){
			seconds = 0;
			calculate_frame_count = 1;
		}
	}

	/* Flip LED */
	if(lit){
		LED_OFF();
		lit = 0;
	}else{
		LED_ON();
		lit = 1;
	}
}
