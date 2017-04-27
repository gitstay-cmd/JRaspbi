#include "bcm2835.h"
#ifndef gpio_h_INCLUDED
#define gpio_h_INCLUDED
#include <stdint.h>

#define GPIO_BASE Peripheral_Base + 0x200000UL

#define GPFSEL0 	0
#define GPFSEL1 	4
#define GPFSEL2 	8
#define GPFSEL3 	0xC
#define GPFSEL4 	0x10
#define GPFSEL5 	0x14
#define GPFSET0		0x1C
#define GPFSET1		0x20
#define GPCLR0		0x28
#define GPCLR1		0x2C
#define GPLEV0		0x34	
#define GPLEV1		0x38
#define GPEDS0		0x40
#define GPEDS1		0x44
#define GPREN0		0x4C
#define GPREN1		0x50
#define GPFEN0		0x58
#define GPFEN1		0x5C
#define GPHEN0		0x64
#define GPHEN1		0x68
#define GPLEN0		0x70
#define GPLEN1		0x74
#define GPAREN0		0x7C
#define GPAREN1		0x80
#define GPAFEN0		0x88
#define GPAFEN1		0x8C
#define GPPUD		0x94
#define GPPUDCLK0	0x98
#define GPPUDCLK1	0x9C

typedef enum {
    RPI_GPIO0 = 0,
    RPI_GPIO1,
    RPI_GPIO2,
    RPI_GPIO3,
    RPI_GPIO4,
    RPI_GPIO5,
    RPI_GPIO6,
    RPI_GPIO7,
    RPI_GPIO8,
    RPI_GPIO9,
    RPI_GPIO10,
    RPI_GPIO11,
    RPI_GPIO12,
    RPI_GPIO13,
    RPI_GPIO14,
    RPI_GPIO15,
    RPI_GPIO16,
    RPI_GPIO17,
    RPI_GPIO18,
    RPI_GPIO19,
    RPI_GPIO20,
    RPI_GPIO21,
    RPI_GPIO22,
    RPI_GPIO23,
    RPI_GPIO24,
    RPI_GPIO25,
    RPI_GPIO26,
    RPI_GPIO27,
    RPI_GPIO28,
    RPI_GPIO29,
    RPI_GPIO30,
    RPI_GPIO31,
    RPI_GPIO32,
    RPI_GPIO33,
    RPI_GPIO34,
    RPI_GPIO35,
    RPI_GPIO36,
    RPI_GPIO37,
    RPI_GPIO38,
    RPI_GPIO39,
    RPI_GPIO40,
    RPI_GPIO41,
    RPI_GPIO42,
    RPI_GPIO43,
    RPI_GPIO44,
    RPI_GPIO45,
    RPI_GPIO46,
    RPI_GPIO47,
    RPI_GPIO48,
    RPI_GPIO49,
    RPI_GPIO50,
    RPI_GPIO51,
    RPI_GPIO52,
    RPI_GPIO53,
} gpio_pin;

typedef enum {
	LO =0,
	HI,
	ON,
	OFF,
	UNKOWN,
} gpio_value;

typedef enum{
	GPIO_INPUT = 0,
	GPIO_OUTPUT,
	GPIO_ALTFUNC0 = 4,
	GPIO_ALTFUNC1,
	GPIO_ALTFUNC2,
	GPIO_ALTFUNC3,
	GPIO_ALTFUNC4 = 3,
	GPIO_ALTFUNC5 = 2,
} gpio_function;

uint32_t getPinValue(gpio_pin gpio);

void setPinOn(gpio_pin gpio);

void setPinOff(gpio_pin gpio);

void SetGpioOutPut(gpio_pin gpio);

#endif // gpio_h_INCLUDED

