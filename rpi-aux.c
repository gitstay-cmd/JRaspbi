#include "rpi-aux.h"
#include "rpi-base.h"
#include "rpi-gpio.h"

static aux_t* auxillary = (aux_t*)AUX_BASE;


aux_t* RPI_GetAux(void){
	return auxillary;
}

#define SYS_FREQ	250000000

void RPI_AuxMiniUartInit(int baud, int bits){
	volatile int i;
	
	auxillary->ENABLES = AUX_ENA_MINIUART;

	auxillary->MU_IER = 0;

	auxillary->MU_CNTL = 0;

	/* Decide between 7 and 8 bit mode*/
	if( bits == 8){
		auxillary->MU_LCR = AUX_MULCR_8BIT_MODE;
	}else {
		auxillary->MU_LCR = 0;
	}

	/*Disable all interrupts from MU and clear the fifos*/
	auxillary->MU_MCR = 0;

	auxillary->MU_IIR = 0xC6;

	/*Transposed calculation from Section 2.2.1 of the ARM peripherals manual */
	auxillary->MU_BAUD = ( SYS_FREQ / ( 8 * baud ) ) - 1;

	/* Setup GPIO 14 and 15 as alternative functions 5 which is UART 1 TXD/RXD. These need to be set before enabling the UART */

	RPI_SetGpioPinFunction( RPI_GPIO14, FS_ALT5);
	RPI_SetGpioPinFunction( RPI_GPIO15, FS_ALT5);
	RPI_GetGpio()->GPPUD = 0;
	for(i =0; i < 150; i++){}
	RPI_GetGpio()->GPPUDCLK0 = (1 << 14);
	for(i =0; i < 150; i++){}
	RPI_GetGpio()->GPPUDCLK0 = 0;
	
	/* Disable flow control, enable transmitter and receiver*/
	auxillary->MU_CNTL = AUX_MUCNTL_TX_ENABLE;
}

void RPI_AuxMiniUartWrite(char c){
	/* Wait until the UART has an empty space in the FIFO */
	while( (auxillary->MU_LSR & AUX_MULSR_TX_EMPTY ) == 0){
	}
	/* Write the character to the FIFO for transmission */
	auxillary->MU_IO = c;
}
