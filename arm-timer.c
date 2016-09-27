#include "arm-timer.h"
#include <stdint.h>

static rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARMTIMER_BASE;

rpi_arm_timer_t* RPI_GetArmTimer(void)
{
		return rpiArmTimer;
}

void RPI_ArmTimerInit(void){
}
