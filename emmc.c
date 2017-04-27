#include "emmc.h"

emmc_d* emmc = (emmc_d*)EMMC_BASE

void emmc_power_off(void){

	emmc->CONTROL0 &= ~(1 << 5);

}
