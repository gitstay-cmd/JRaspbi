#ifndef uart_h_INCLUDED
#define uart_h_INCLUDED
#include "gpio.h"
#include "mmio.h"
#include <stdint.h>
#include <stddef.h>

#define UART0_BASE (GPIO_BASE + 0x1000);

typedef struct UART {
    uint32_t UART0_DR;
    uint32_t UART0_RSRECR;
    uint32_t UART0_FR;
    uint32_t UART0_ILPR;
    uint32_t UART0_IBRD;
    uint32_t UART0_FBRD;
    uint32_t UART0_LCRH;
    uint32_t UART0_CR;
    uint32_t UART0_IFLS;
    uint32_t UART0_IMSC;
    uint32_t UART0_RIS;
    uint32_t UART0_MIS;
    uint32_t UART0_ICR;
    uint32_t UART0_DMACR;
    uint32_t UART0_ITCR;
    uint32_t UART0_ITIP;
    uint32_t UART0_ITOP;
    uint32_t UART0_TDR;
}uart;

void uart_putc(unsigned char c);

unsigned char uart_getc();

void uart_puts(const char * str);
#endif // uart_h_INCLUDED

