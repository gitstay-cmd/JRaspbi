#ifndef RPI_UART_H
#define RPI_UART_H
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum {
	GPIO_BASE = 0x20200000,

	GPPUD = (GPIO_BASE +0x94),

	GPPUDCLK0 = (GPIO_BASE + 0x98),

	UART0_BASE = 0x20201000,

	UART0_DR		= (UART0_BASE + 0x00),
	UART0_RSRECR	= (UART0_BASE + 0x04),
	UART0_FR		= (UART0_BASE + 0x18),
	UART0_ILPR		= (UART0_BASE + 0x20),
	UART0_IBRD		= (UART0_BASE + 0x24),
	UART0_FBRD		= (UART0_BASE + 0x28),
	UART0_LCRH		= (UART0_BASE + 0x2C),
	UART0_CR		= (UART0_BASE + 0x30),
	UART0_IFLS		= (UART0_BASE + 0x34),
	UART0_IMSC		= (UART0_BASE + 0x38),
	UART0_RIS		= (UART0_BASE + 0x3C),
	UART0_MIS		= (UART0_BASE + 0x40),
	UART0_ICR		= (UART0_BASE + 0x44),
	UART0_DMACR		= (UART0_BASE + 0x48),
	UART0_ITCR		= (UART0_BASE + 0x80),
	UART0_ITIP		= (UART0_BASE + 0x84),
	UART0_ITOP		= (UART0_BASE + 0x88),
	UART0_TDR		= (UART0_BASE + 0x8C),
};

static inline void mmio_write(uint32_t reg, uint32_t data);
static inline uint32_t mmio_read(uint32_t reg);
static inline void delay(int32_t count);
size_t strlen(const char* str);
void uart_init();
void uart_putc(unsigned char byte);
unsigned char uart_getc();
void uart_write(const unsigned char* buffer, size_t size);
void uart_puts(const char* str);

#endif
