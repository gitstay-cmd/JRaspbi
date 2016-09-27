/**
 * @file bcm2835.h
 *
 */
/* Copyright (C) 2014 by Arjan van Vught <pm @ http://www.raspberrypi.org/forum/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef BCM2835_H_
#define BCM2835_H_

/*
 *  Removed test of architecture flag to determine if target is RasPi2 or
 *  older version.  That information should be set in the project-specific
 *  target.h file.
 */


/*
 *  Define the peripheral base address, based on the target type.
 *  Raspberry Pi 2 boards use a different chip with a different
 *  base address, compared to the older Raspberry Pi boards.
 */
#if defined ( RPI2 )
#define BCM2835_PERI_BASE		0x3F000000
#else
#define BCM2835_PERI_BASE		0x20000000
#endif

#define BCM2835_ST_BASE			(BCM2835_PERI_BASE + 0x3000)
#define BCM2835_IRQ_BASE		(BCM2835_PERI_BASE + 0xB200)
#define BCM2835_TIMER_BASE		(BCM2835_PERI_BASE + 0xB400)
#define BCM2835_MAILBOX_BASE 	(BCM2835_PERI_BASE + 0xB880)
#define BCM2835_PM_WDOG_BASE	(BCM2835_PERI_BASE + 0x100000)
#define BCM2835_GPIO_BASE		(BCM2835_PERI_BASE + 0x200000)
#define BCM2835_SPI0_BASE		(BCM2835_PERI_BASE + 0x204000)	// Base Physical Address of the SPI0 registers
#define BCM2835_PL011_BASE		(BCM2835_PERI_BASE + 0x201000)	// Base Physical Address of the PL011 registers
#define BCM2835_UART1_BASE		(BCM2835_PERI_BASE + 0x215000)	// Base Physical Address of the UART1 registers
#define BCM2835_EMMC_BASE		(BCM2835_PERI_BASE + 0x300000)	// Base Physical Address of the EMMC registers
#define BCM2835_BSC1_BASE		(BCM2835_PERI_BASE + 0x804000)	// Base Physical Address of the BSC1 registers
#define BCM2835_BSC2_BASE		(BCM2835_PERI_BASE + 0x805000)	// Base Physical Address of the BSC2 registers

#include <stdint.h>

// https://github.com/raspberrypi/linux/blob/rpi-3.6.y/arch/arm/mach-bcm2708/include/mach/platform.h
#define ARM_IRQ1_BASE		0						//
#define INTERRUPT_TIMER1	(ARM_IRQ1_BASE + 1)		//
#define INTERRUPT_TIMER3	(ARM_IRQ1_BASE + 3)		//
#define INTERRUPT_AUX		(ARM_IRQ1_BASE + 29)	//

#define ARM_IRQ2_BASE		32						//
#define INTERRUPT_VC_UART	(ARM_IRQ2_BASE + 25)	// UART Interrupt



/* ============= System timer ============ */
/* The system timer is a free-running 1 MHz counter with 4 compare registers,
 * essentially creating four timer channels.
 *
 * Register CLO is the low 32 bits of the free-running timer and can be
 * used for delta timing at microsecond resolution.
 *
 * Timer/counter channels 0 and 2 are reserved for use by the GPU; do not
 * use them in your code!
 */
typedef struct
{
	volatile uint32_t		CS;			// 0x00	Control/Status 
	volatile uint32_t		CLO;		// 0x04	Counter Lower 32 bits
	volatile uint32_t		CHI;		// 0x08	Counter Higher 32 bits
	volatile uint32_t		C0;			// 0x0C	Compare 0.  DO NOT USE; is used by GPU.
	volatile uint32_t		C1;			// 0x10	Compare 1
	volatile uint32_t		C2;			// 0x14	Compare 2.  DO NOT USE; is used by GPU.
	volatile uint32_t		C3;			// 0x18	Compare 3
} BCM2835_ST_TypeDef;

/*  Define masks for selecting timer compare flags.  Use these with the Control/Status
 *  register to test or clear match compare flags.
 */
#define BCM2835_ST_CS_M0		((uint32_t)(1 << 0))	// System Timer Match 0. DO NOT USE; is used by GPU.
#define BCM2835_ST_CS_M1		((uint32_t)(1 << 1))	// System Timer Match 1
#define BCM2835_ST_CS_M2		((uint32_t)(1 << 2))	// System Timer Match 2. DO NOT USE; is used by GPU.
#define BCM2835_ST_CS_M3		((uint32_t)(1 << 3))	// System Timer Match 3





/* ============= UART ============ */
typedef struct
{
	volatile uint32_t		IRQ;		// 0x00
	volatile uint32_t		ENABLE;		// 0x04
	volatile uint32_t		PAD[14];	// 0x08
	volatile uint32_t		IO;			// 0x40
	volatile uint32_t		IER;		// 0x44
	volatile uint32_t		IIR;		// 0x48
	volatile uint32_t		LCR;		// 0x4C
	volatile uint32_t		MCR;		// 0x50
	volatile uint32_t		LSR;		// 0x54
	volatile uint32_t		MSR;		// 0x58
	volatile uint32_t		SCR;		// 0x5C
	volatile uint32_t		CNTL;		// 0x60
	volatile uint32_t		STAT;		// 0x64
	volatile uint32_t		BAUD;		// 0x68
} BCM2835_UART_TypeDef;



/// 13.4 Register View
typedef struct
{
	volatile uint32_t		DR;			// 0x00, Data Register
	volatile uint32_t		RSRECR;		// 0x04, Receive status register/error clear register
	volatile uint32_t		PAD[4];		// 0x08, Padding
	volatile uint32_t		FR;			// 0x18, Flag register
	volatile uint32_t		RES1;		// 0x1C, Reserved
	volatile uint32_t		ILPR;		// 0x20, not in use
	volatile uint32_t		IBRD;		// 0x24
	volatile uint32_t		FBRD;		// 0x28
	volatile uint32_t		LCRH;		// 0x2C
	volatile uint32_t		CR;			// 0x30
	volatile uint32_t		IFLS;		// 0x34
	volatile uint32_t		IMSC;		// 0x38
	volatile uint32_t		RIS;		// 0x3C
	volatile uint32_t		MIS;		// 0x40
	volatile uint32_t		ICR;		// 0x44
	volatile uint32_t		DMACR;		// 0x48
} BCM2835_PL011_TypeDef;




/* ============= GPIO ============ */
typedef struct
{
	volatile uint32_t		GPFSEL0;	// 0x00, GPIO Function Select 0
	volatile uint32_t		GPFSEL1;	// 0x04, GPIO Function Select 1
	volatile uint32_t		GPFSEL2;	// 0x08, GPIO Function Select 2
	volatile uint32_t		GPFSEL3;	// 0x0C, GPIO Function Select 3
	volatile uint32_t		GPFSEL4;	// 0x10, GPIO Function Select 4
	volatile uint32_t		GPFSEL5;	// 0x14, GPIO Function Select 5
	volatile uint32_t		RES1;		// 0x18, Reserved
	volatile uint32_t		GPSET0;		// 0x1C, GPIO Pin Output Set 0
	volatile uint32_t		GPSET1;		// 0x20, GPIO Pin Output Set 1
	volatile uint32_t		RES2;		// 0x24, Reserved
	volatile uint32_t		GPCLR0;		// 0x28, GPIO Pin Output Clear 0
	volatile uint32_t		GPCLR1;		// 0x2C, GPIO Pin Output Clear 1
	volatile uint32_t		RES3;		// 0x30, Reserved
	volatile uint32_t		GPLEV0;		// 0x34, GPIO Pin Level 0
	volatile uint32_t		GPLEV1;		// 0x38, GPIO Pin Level 1
	volatile uint32_t		RES4;		// 0x3C, Reserved
	volatile uint32_t		GPEDS0;		// 0x40, GPIO Pin Event Detect Status 0
	volatile uint32_t		GPEDS1;		// 0x44, GPIO Pin Event Detect Status 1
	volatile uint32_t		RES5;		// 0x48, Reserved
	volatile uint32_t		GPREN0;		// 0x4C, GPIO Pin Rising Edge Detect Enable 0
	volatile uint32_t		GPREN1;		// 0x50, GPIO Pin Rising Edge Detect Enable 1
	volatile uint32_t		RES6;		// 0x54, Reserved
	volatile uint32_t		GPFEN0;		// 0x58, GPIO Pin Falling Edge Detect Enable 0
	volatile uint32_t		GPFEN1;		// 0x5C, GPIO Pin Falling Edge Detect Enable 1
	volatile uint32_t		RES7;		// 0x60, Reserved
	volatile uint32_t		GPHEN0;		// 0x64, GPIO Pin High Detect Enable 0
	volatile uint32_t		GPHEN1;		// 0x68, GPIO Pin High Detect Enable 1
	volatile uint32_t		RES8;		// 0x6C, Reserved
	volatile uint32_t		GPLEN0;		// 0x70, GPIO Pin Low Detect Enable 0
	volatile uint32_t		GPLEN1;		// 0x74, GPIO Pin Low Detect Enable 1
	volatile uint32_t		RES9;		// 0x78, Reserved
	volatile uint32_t		AREN[2];	// 0x7C
	volatile uint32_t		RES10;		// 0x84, Reserved
	volatile uint32_t		AFEN[2];	// 0x88
	volatile uint32_t		RES11;		// 0x90
	volatile uint32_t		GPPUD; 		// 0x94, GPIO Pin Pull-up/down Enable
	volatile uint32_t		GPPUDCLK0;	// 0x98, GPIO Pin Pull-up/down Enable Clock 0
	volatile uint32_t		GPPUDCLK1;	// 0x9C, GPIO Pin Pull-up/down Enable Clock 1
} BCM2835_GPIO_TypeDef;




/* ============= SPI ============ */
typedef struct
{
	volatile uint32_t		CS;		// 0x00, SPI Master Control and Status
	volatile uint32_t		FIFO;	// 0x04, SPI Master TX and RX FIFOs
	volatile uint32_t		CLK;	// 0x08, SPI Master Clock Divider
	volatile uint32_t		DLEN;	// 0x0C, SPI Master Data Length
	volatile uint32_t		LTOH;	// 0x10, SPI LOSSI mode TOH
	volatile uint32_t		DC;		// 0x14, SPI DMA DREQ Controls
} BCM2835_SPI_TypeDef;



/* =========== Broadcom Serial Controller (BSC) ================ */
/*  This is a proprietary serial bus that is compatible with I2C */
typedef struct
{
	volatile uint32_t		C;		// 0x00, BSC Master Control
	volatile uint32_t		S;		// 0x04, BSC Master Status
	volatile uint32_t		DLEN;	// 0x08, BSC Master Data Length
	volatile uint32_t		A;		// 0x0C, BSC Master Slave Address
	volatile uint32_t		FIFO;	// 0x10, BSC Master Data FIFO
	volatile uint32_t		DIV;	// 0x14, BSC Master Clock Divider
	volatile uint32_t		DEL;	// 0x18, BSC Master Data Delay
	volatile uint32_t		CLKT;	// 0x1C, BSC Master Clock Stretch Timeout
} BCM2835_BSC_TypeDef;



/* =========== IRQ =============== */
typedef struct
{
	volatile uint32_t		IRQ_BASIC_PENDING;		// 0x00
	volatile uint32_t		IRQ_PENDING1;			// 0x04
	volatile uint32_t		IRQ_PENDING2;			// 0x08
	volatile uint32_t		FIQ_CONTROL;			// 0x0C
	volatile uint32_t		IRQ_ENABLE1;			// 0x10
	volatile uint32_t		IRQ_ENABLE2;			// 0x14
	volatile uint32_t		IRQ_BASIC_ENABLE;		// 0x18
	volatile uint32_t		IRQ_DISABLE1;			// 0x1C
	volatile uint32_t		IRQ_DISABLE2;			// 0x20
	volatile uint32_t		IRQ_BASIC_DISABLE;		// 0x24
} BCM2835_IRQ_TypeDef;

#define  IRQ_TIMER_MASK				(1 << 0)
#define  IRQ_MAILBOX_MASK			(1 << 1)
#define  IRQ_DOORBELL_0_MASK		(1 << 2)
#define  IRQ_DOORBELL_1_MASK		(1 << 3)
#define  IRQ_GPU_0_HALTED_MASK		(1 << 4)
#define	 IRQ_GPU_1_HALTED_MASK		(1 << 5)
#define  IRQ_ACCESS_ERROR_1_MASK	(1 << 6)
#define  IRQ_ACCESS_ERROR_0_MASK	(1 << 7)

typedef enum
{
	// ARM_IRQ1_BASE
	BCM2835_TIMER1_IRQn = 1 << (INTERRUPT_TIMER1 - ARM_IRQ1_BASE),		//
	BCM2835_TIMER3_IRQn = 1	<< (INTERRUPT_TIMER3 - ARM_IRQ1_BASE),		//
	BCM2835_UART1_IRQn  = 1 << (INTERRUPT_AUX - ARM_IRQ1_BASE),			//
	// ARM_IRQ2_BASE
	BCM2835_VC_UART_IRQn  = 1 << (INTERRUPT_VC_UART - ARM_IRQ2_BASE)	//
} BCM2835_IRQn_TypeDef;

#define BCM2835_FIQ_ENABLE	(1 << 7)// 0x80




/* =========== Timer =============== */
/* This is the ARM Timer, not the system timer; see Chapter 14
 * of the Broadcom BCM2835 ARM Peripherals manual (page 196 or so).
 */
typedef struct
{
	volatile  uint32_t		LOAD;			// 0x00
	volatile  uint32_t		VALUE;			// 0x04
	volatile  uint32_t		CTRL;			// 0x08
	volatile  uint32_t		IRQ_CLRACK;		// 0x0c
	volatile  uint32_t		RAW_IRQ;		// 0x10
	volatile  uint32_t		MASKED_IRQ;		// 0x14
	volatile  uint32_t		RELOAD;			// 0x18
} BCM2835_TIMER_TypeDef;


#define TIMER_CTRL_23BIT			(1 << 1)

#define TIMER_CTRL_PRESCALE_MASK	(3 << 2)
#define TIMER_CTRL_PRESCALE_1		(0 << 2)
#define TIMER_CTRL_PRESCALE_16		(1 << 2)
#define TIMER_CTRL_PRESCALE_256		(2 << 2)

#define TIMER_CTRL_INT_ENABLE		(1 << 5)
#define TIMER_CTRL_INT_DISABLE		(0 << 5)
#define TIMER_CTRL_ENABLE			(1 << 7)
#define TIMER_CTRL_DISABLE			(0 << 7)



/* =========== Mailbox =============== */
typedef struct
{
	volatile uint32_t		READ;			// 0x00
	volatile uint32_t		RES1;			// 0x04
	volatile uint32_t		RES2;			// 0x08
	volatile uint32_t		RES3;			// 0x0C
	volatile uint32_t		PEEK;			// 0x10
	volatile uint32_t		SENDER;			// 0x14
	volatile uint32_t		STATUS;			// 0x18
	volatile uint32_t		CONFIG;			// 0x1C
	volatile uint32_t		WRITE;			// 0x20
} BCM2835_MAILBOX_TypeDef;



/* =========== Watchdog =============== */
typedef struct
{
	volatile uint32_t		UNKWOWN0[7];	// 0x00
	volatile uint32_t		RSTC;			// 0x1C
	volatile uint32_t		UNKWOWN1;		// 0x20
	volatile uint32_t		WDOG;			// 0x24
} BCM2835_PM_WDOG_TypeDef;



/* =========== External Mass Media Controller (EMMC) =============== */
typedef struct
{
	volatile uint32_t		ARG2;				// 0x00
	volatile uint32_t		BLKSIZECNT;			// 0x04
	volatile uint32_t		ARG1;				// 0x08
	volatile uint32_t		CMDTM;				// 0x0C
	volatile uint32_t		RESP0;				// 0x10
	volatile uint32_t		RESP1;				// 0x14
	volatile uint32_t		RESP2;				// 0x18
	volatile uint32_t		RESP3;				// 0x1C
	volatile uint32_t		DATA;				// 0x20
	volatile uint32_t		STATUS;				// 0x24
	volatile uint32_t		CONTROL0;			// 0x28
	volatile uint32_t		CONTROL1;			// 0x2C
	volatile uint32_t		INTERRUPT;			// 0x30
	volatile uint32_t		IRPT_MASK;			// 0x34
	volatile uint32_t		IRPT_EN;			// 0x38
	volatile uint32_t		CONTROL2;			// 0x3C
	volatile uint32_t		CAPABILITIES_0;		// 0x40
	volatile uint32_t		CAPABILITIES_1;		// 0x44
	volatile uint32_t		NOTINUSE1[2];
	volatile uint32_t		FORCE_IRPT;			// 0x50
	volatile uint32_t		NOTINUSE2[7];
	volatile uint32_t		BOOT_TIMEOUT;		// 0x70
	volatile uint32_t		DBG_SEL;			// 0x74
	volatile uint32_t		NOTINUSE3[2];
	volatile uint32_t		EXRDFIFO_CFG;		// 0x80
	volatile uint32_t		EXRDFIFO_EN;		// 0x84
	volatile uint32_t		TUNE_STEP;			// 0x88
	volatile uint32_t		TUNE_STEPS_STD;		// 0x8C
	volatile uint32_t		TUNE_STEPS_DDR;		// 0x90
	volatile uint32_t		NOTINUSE4[23];
	volatile uint32_t		SPI_INT_SPT;		// 0xF0
	volatile uint32_t		NOTINUSE5[2];
	volatile uint32_t		SLOTISR_VER;		// 0xFC
} BCM2835_EMMC_TypeDef;



/*  Define pointers to the various peripheral register blocks  */

#define BCM2835_ST			((BCM2835_ST_TypeDef *)   BCM2835_ST_BASE)			// Base register address for SYSTEM TIMER
#define BCM2835_IRQ			((BCM2835_IRQ_TypeDef *)  BCM2835_IRQ_BASE)			// Base register address for IRQ
#define BCM2835_MAILBOX		((BCM2835_MAILBOX_TypeDef *) BCM2835_MAILBOX_BASE)	// Base register address for MAILBOX
#define BCM2835_PM_WDOG		((BCM2835_PM_WDOG_TypeDef *) BCM2835_PM_WDOG_BASE)	// Base register address for WATCHDOG
#define BCM2835_GPIO		((BCM2835_GPIO_TypeDef *) BCM2835_GPIO_BASE)		// Base register address for GPIO
#define BCM2835_SPI0		((BCM2835_SPI_TypeDef *)  BCM2835_SPI0_BASE)		// Base register address for SPI
#define BCM2835_PL011		((BCM2835_PL011_TypeDef *) BCM2835_PL011_BASE)		// Base register address for PL011
#define BCM2835_UART1		((BCM2835_UART_TypeDef *) BCM2835_UART1_BASE)		// Base register address for UART1
#define BCM2835_EMMC		((BCM2835_EMMC_TypeDef *) BCM2835_EMMC_BASE)		// Base register address for EMMC
#define BCM2835_BSC1		((BCM2835_BSC_TypeDef *)  BCM2835_BSC1_BASE)		// Base register address for I2C (BSC1)
#define BCM2835_BSC2		((BCM2835_BSC_TypeDef *)  BCM2835_BSC2_BASE)		// Base register address for I2C (BSC2)
#define BCM2835_TIMER		((BCM2835_TIMER_TypeDef *) BCM2835_TIMER_BASE)		// base register address for ARM TIMER



#if defined( RPI2 )
#define isb() asm volatile ("isb" ::: "memory")
#define dsb() asm volatile ("dsb" ::: "memory");
#define dmb() asm volatile ("dmb" ::: "memory")
#else
#define isb() asm volatile ("mcr p15, #0, %[zero], c7, c5,  #4" : : [zero] "r" (0) )
#define dsb() asm volatile ("mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0) )
#define dmb() asm volatile ("mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0) )
#endif


/*
 *  Define a function call for enabling interrupts.  Actual code is in
 *  start.s.
 */
void			_enable_interrupts(void);


/*
 *  Commented out the following timer routines and interrupt support
 *  declarations.  Timer routines belong in a dedicated timer.h file.
 *  Low-level interrupt support routines belong in a dedicated
 *  interrupt header file or in the start.s (assembly) file.
 */
#if 0
/**
 * @ingroup TIMER
 *
 * @param offset_micros
 * @param micros
 */
extern void bcm2835_st_delay(uint64_t offset_micros, uint64_t micros);

/**
 * @ingroup TIMER
 *
 * @return
 */
inline static uint64_t bcm2835_st_read(void)
{
	return *(volatile uint64_t *)(BCM2835_ST_BASE + 0x04);
}

/**
 * @ingroup TIMER
 *
 * @param usec
 */
extern void udelay(const uint64_t usec);

/**
 *
 */
extern void __disable_fiq(void);
/**
 *
 */
extern void __enable_fiq(void);

/**
 *
 */
extern void __disable_irq(void);

/**
 *
 */
extern void __enable_irq(void);

/**
 *
 */
extern void memory_barrier(void);
#endif



#endif /* BCM2835_H_ */
