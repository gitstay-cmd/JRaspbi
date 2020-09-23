#include "gpio.h"
#include "uart.h"
#include <stdint.h>

uart* uart0 = (uart*)UART0_BASE;

void uart_init(void){

	uart0->UART0_CR = 0x00000000;
	uart0->UART0_ICR = 0x7FF;
	uart0->UART0_IBRD = 1;
	uart0->UART0_FBRD = 40;
	uart0->UART0_LCRH = ((1 <<4) | (1 << 5) | (1<<6));
	uart0->UART0_IMSC = ((1<<1)|(1<<4)|(1<<5)|(1<<6)|(1<<7)|(1<<8)|(1<<9)|(1<<10));

	uart0->UART0_CR = ((1<<0) | (1<<8) | (1<<9));
	

}

void uart_putc(unsigned char c){

	while(uart0->UART0_FR & (1 <<5)){}

	uart0->UART0_DR = c;

}

unsigned char uart_getc(){

	while( uart0->UART0_FR & (1 << 4)){}

	return uart0->UART0_DR;

}

void uart_puts(const char * str){

	for( size_t i = 0; str[i] != '\0'; i++){

    	uart_putc((unsigned char)str[i]);
	}
}
