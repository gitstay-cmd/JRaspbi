#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "rpi-uart.h"
#include "timer.h"

static inline void mmio_write(uint32_t reg, uint32_t data){
	*(volatile uint32_t*)reg = data;
}

static inline uint32_t mmio_read(uint32_t reg){
	   return *(volatile uint32_t *)reg;
}

static inline void delay(int32_t count){
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : : [count]"r"(count) : "cc");
}
size_t strlen(const char* str){
	size_t ret = 0;
	while(str[ret] != 0){
		ret++;
	}
	return ret;
}

void uart_init(){
	mmio_write(UART0_CR, 0x00000000);

	mmio_write(GPPUD, 0x00000000);
	delay(150);

	mmio_write(GPPUDCLK0, (1<<14) | (1 << 15));
	delay(150);

	mmio_write(GPPUDCLK0, 0x00000000);

	mmio_write(UART0_ICR, 0x7FF);

	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);

	mmio_write(UART0_LCRH, (1 <<4 ) | (1 << 5) | (1 << 6));
	

	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6 ) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9 ));
}

void uart_putc(unsigned char byte){
	while(mmio_read(UART0_FR) & (1 << 4) ){}
	mmio_write(UART0_DR, byte);
}

unsigned char uart_getc(){
	while (mmio_read(UART0_FR) & (1 << 4)){}
	return mmio_read(UART0_DR);
}

void uart_write(const unsigned char* buffer, size_t size){
	size_t i;
	for( i = 0; i < size; i++){
		uart_putc(buffer[i]);
	}
}

void uart_puts(const char* str){
	uart_write((const unsigned char*) str, strlen(str));
}

