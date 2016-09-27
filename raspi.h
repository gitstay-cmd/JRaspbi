/*
 *  raspi.h      register definitions for some of the RasPi peripherals
 *
 *  This file is a blend of several subsystem-specific header files
 *  taken from Brian Sidebotham's excellent series of tutorials on
 *  bare-metal Raspberry Pi development.  You can find the original
 *  files here:
 *  https://github.com/BrianSidebotham/arm-tutorial-rpi/tree/master/part-4/armc-013
 */

/*
	
	Part of the Raspberry-Pi Bare Metal Tutorials
	Copyright (c) 2013-2015, Brian Sidebotham
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	
	1. Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
	
	2. Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
	
*/
	
#ifndef RASPI_H
#define RASPI_H

#include  <stdint.h>
	
#ifdef RPI2
	#define PERIPHERAL_BASE 0x3F000000UL
#else
	#define PERIPHERAL_BASE 0x20000000UL
#endif


/*
 *  Define pin assignments specific to the target.  Refer to the
 *  project-specific file target.h for selecting the target board.
 */
#if defined( RPIBPLUS ) || defined( RPI2 )

#define LED_GPFSEL      GPFSEL4
#define LED_GPFBIT      21
#define LED_GPSET       GPSET1
#define LED_GPCLR       GPCLR1
#define LED_GPIO_BIT    15

#else

#define LED_GPFSEL      GPFSEL1
#define LED_GPFBIT      18
#define LED_GPSET       GPSET0
#define LED_GPCLR       GPCLR0
#define LED_GPIO_BIT    16

#define UART0_GPFSEL	GPFSEL1
#define UART0_GPFBIT_TX	
#define UART0_GPSET		GPSET0
#define UART0_GPCLR		GPCLR0
#define UART0_GPIO_BIT_TX	

/*
 *  These macros map the name of a pin on the expansion connector (P1) to
 *  the BCM2835 GPIO bit number.
 *
 *  The original code from Brian Sidebotham include _V2_ in these names,
 *  intending them to be for Revision 2 of the original RasPi.  However,
 *  release of Version 2 of the RasPi makes that naming confusing.  I've
 *  removed _V2_; understand that these macros all refer to the pin naming
 *  used by the RasPi B.  If you need support for other version, add
 *  a block with suitable naming.
 */
#define RPI_GPIO_P1_03      2  // Version 2, Pin P1-03, SDA when I2C in use
#define RPI_GPIO_P1_05      3  // Version 2, Pin P1-05, SCL when I2C in use
#define RPI_GPIO_P1_07      4  // Version 2, Pin P1-07
#define RPI_GPIO_P1_08     14  // Version 2, Pin P1-08, defaults to ALT function 0 PL011_TXD
#define RPI_GPIO_P1_10     15  // Version 2, Pin P1-10, defaults to ALT function 0 PL011_RXD
#define RPI_GPIO_P1_11     17  // Version 2, Pin P1-11
#define RPI_GPIO_P1_12     18  // Version 2, Pin P1-12
#define RPI_GPIO_P1_13     27  // Version 2, Pin P1-13
#define RPI_GPIO_P1_15     22  // Version 2, Pin P1-15
#define RPI_GPIO_P1_16     23  // Version 2, Pin P1-16
#define RPI_GPIO_P1_18     24  // Version 2, Pin P1-18
#define RPI_GPIO_P1_19     10  // Version 2, Pin P1-19, MOSI when SPI0 in use
#define RPI_GPIO_P1_21      9  // Version 2, Pin P1-21, MISO when SPI0 in use
#define RPI_GPIO_P1_22     25  // Version 2, Pin P1-22
#define RPI_GPIO_P1_23     11  // Version 2, Pin P1-23, CLK when SPI0 in use
#define RPI_GPIO_P1_24      8  // Version 2, Pin P1-24, CE0 when SPI0 in use
#define RPI_GPIO_P1_26      7  // Version 2, Pin P1-26, CE1 when SPI0 in use

#endif


// RPi Version 2
#define RPI_V2_GPIO_P1_03      2  // Version 2, Pin P1-03, SDA when I2C in use
#define RPI_V2_GPIO_P1_05      3  // Version 2, Pin P1-05, SCL when I2C in use
#define RPI_V2_GPIO_P1_07      4  // Version 2, Pin P1-07
#define RPI_V2_GPIO_P1_08     14  // Version 2, Pin P1-08, defaults to ALT function 0 PL011_TXD
#define RPI_V2_GPIO_P1_10     15  // Version 2, Pin P1-10, defaults to ALT function 0 PL011_RXD
#define RPI_V2_GPIO_P1_11     17  // Version 2, Pin P1-11
#define RPI_V2_GPIO_P1_12     18  // Version 2, Pin P1-12
#define RPI_V2_GPIO_P1_13     27  // Version 2, Pin P1-13
#define RPI_V2_GPIO_P1_15     22  // Version 2, Pin P1-15
#define RPI_V2_GPIO_P1_16     23  // Version 2, Pin P1-16
#define RPI_V2_GPIO_P1_18     24  // Version 2, Pin P1-18
#define RPI_V2_GPIO_P1_19     10  // Version 2, Pin P1-19, MOSI when SPI0 in use
#define RPI_V2_GPIO_P1_21      9  // Version 2, Pin P1-21, MISO when SPI0 in use
#define RPI_V2_GPIO_P1_22     25  // Version 2, Pin P1-22
#define RPI_V2_GPIO_P1_23     11  // Version 2, Pin P1-23, CLK when SPI0 in use
#define RPI_V2_GPIO_P1_24      8  // Version 2, Pin P1-24, CE0 when SPI0 in use
#define RPI_V2_GPIO_P1_26      7  // Version 2, Pin P1-26, CE1 when SPI0 in use


/*
 *  Macros for controlling the OK LED
 */
#define LED_ON() do { BCM2835_GPIO->LED_GPCLR = ( 1 << LED_GPIO_BIT ); } while( 0 )
#define LED_OFF() do { BCM2835_GPIO->LED_GPSET = ( 1 << LED_GPIO_BIT ); } while( 0 )



typedef volatile uint32_t			rpi_reg_rw_t;
typedef volatile const uint32_t		rpi_reg_ro_t;
typedef volatile uint32_t			rpi_reg_wo_t;
	
typedef volatile uint64_t			rpi_wreg_rw_t;
typedef volatile const uint64_t		rpi_wreg_ro_t;
	
/*  ============== ARM timer ==============  */

/** @brief See the documentation for the ARM side timer (Section 14 of the
BCM2835 Peripherals PDF) */
#define RPI_ARMTIMER_BASE ( PERIPHERAL_BASE + 0xB400 )
	
/** @brief 0 : 16-bit counters - 1 : 23-bit counter */
#define RPI_ARMTIMER_CTRL_23BIT			(1 << 1)
	
#define RPI_ARMTIMER_CTRL_PRESCALE_1	(0 << 2)
#define RPI_ARMTIMER_CTRL_PRESCALE_16	(1 << 2)
#define RPI_ARMTIMER_CTRL_PRESCALE_256	(2 << 2)
	
/** @brief 0 : Timer interrupt disabled - 1 : Timer interrupt enabled */
#define RPI_ARMTIMER_CTRL_INT_ENABLE	(1 << 5)
#define RPI_ARMTIMER_CTRL_INT_DISABLE	(0 << 5)
	
/** @brief 0 : Timer disabled - 1 : Timer enabled */
#define RPI_ARMTIMER_CTRL_ENABLE		(1 << 7)
#define RPI_ARMTIMER_CTRL_DISABLE		(0 << 7)
	
	
/** @brief Section 14.2 of the BCM2835 Peripherals documentation details
the register layout for the ARM side timer */
typedef struct
{
	
	/** The timer load register sets the time for the timer to count down.
	This value is loaded into the timer value register after the load
	register has been written or if the timer-value register has counted
	down to 0. */
	volatile uint32_t					Load;
	
	/** This register holds the current timer value and is counted down when
	the counter is running. It is counted down each timer clock until the
	value 0 is reached. Then the value register is re-loaded from the
	timer load register and the interrupt pending bit is set. The timer
	count down speed is set by the timer pre-divide register. */
	volatile uint32_t					Value;
	
	/** The standard SP804 timer control register consist of 8 bits but in the
	BCM implementation there are more control bits for the extra features.
	Control bits 0-7 are identical to the SP804 bits, albeit some
	functionality of the SP804 is not implemented. All new control bits
	start from bit 8 upwards. */
	volatile uint32_t					Control;
	
	/** The timer IRQ clear register is write only. When writing this register
	the interrupt-pending bit is cleared. When reading this register it
	returns 0x544D5241 which is the ASCII reversed value for "ARMT". */
	volatile uint32_t					IRQClear;
	
	/** The raw IRQ register is a read-only register. It shows the status of
	the interrupt pending bit. 0 : The interrupt pending bits is clear.
	1 : The interrupt pending bit is set.
	
	The interrupt pending bits is set each time the value register is
	counted down to zero. The interrupt pending bit can not by itself
	generates interrupts. Interrupts can only be generated if the
	interrupt enable bit is set. */
	volatile uint32_t					RAWIRQ;
	
	/** The masked IRQ register is a read-only register. It shows the status
	of the interrupt signal. It is simply a logical AND of the interrupt
	pending bit and the interrupt enable bit. 0 : Interrupt line not
	asserted. 1 :Interrupt line is asserted, (the interrupt pending and
	the interrupt enable bit are set.) */
	volatile uint32_t					MaskedIRQ;
	
	/** This register is a copy of the timer load register. The difference is
	that a write to this register does not trigger an immediate reload of
	the timer value register. Instead the timer load register value is
	only accessed if the value register has finished counting down to
	zero. */
	volatile uint32_t					Reload;
	
	/** The Pre-divider register is not present in the SP804. The pre-divider
	register is 10 bits wide and can be written or read from. This
	register has been added as the SP804 expects a 1MHz clock which we do
	not have. Instead the pre-divider takes the APB clock and divides it
	down according to:
	
	timer_clock = apb_clock/(pre_divider+1)
	
	The reset value of this register is 0x7D so gives a divide by 126. */
	volatile uint32_t					PreDivider;
	
	/** The free running counter is not present in the SP804. The free running
	counter is a 32 bits wide read only register. The register is enabled
	by setting bit 9 of the Timer control register. The free running
	counter is incremented immediately after it is enabled. The timer can
	not be reset but when enabled, will always increment and roll-over.
	
	The free running counter is also running from the APB clock and has
	its own clock pre-divider controlled by bits 16-23 of the timer
	control register.
	
	This register will be halted too if bit 8 of the control register is
	set and the ARM is in Debug Halt mode. */
	volatile uint32_t				FreeRunningCounter;
	
	}  rpi_arm_timer_t;
	
	
/*
 *  RPI_GetArmTimer      get pointer to register set for accessing timer
 *
 *  This routine returns a pointer to the timer register set.  Changes to
 *  fields within this register set immediately alter the timer registers.
 */
extern  rpi_arm_timer_t			*RPI_GetArmTimer(void);

/*
 *  RPI_ArmTimerInit      initialize the ARM timer system
 */
extern void						RPI_ArmTimerInit(void);



/*  ==============  GPIO  =============  */

/** The base address of the GPIO peripheral (ARM Physical Address) */
#define RPI_GPIO_BASE		(PERIPHERAL_BASE + 0x200000UL)
	
	
#define RPI_GPIO_FSEL0_00_INPUT			0
#define RPI_GPIO_FSEL0_00_OUTPUT		1
	
#define RPI_GPIO_FSEL0_01_INPUT			(0 << 3)
#define RPI_GPIO_FSEL0_01_OUTPUT		(1 << 3)
	
#define RPI_GPIO_FSEL0_02_INPUT			(0 << 6)
#define RPI_GPIO_FSEL0_02_OUTPUT		(1 << 6)
	
#define RPI_GPIO_FSEL0_03_INPUT			(0 << 9)
#define RPI_GPIO_FSEL0_03_OUTPUT		(1 << 9)
	
#define RPI_GPIO_FSEL0_04_INPUT			(0 << 12)
#define RPI_GPIO_FSEL0_04_OUTPUT		(1 << 12)
	
#define RPI_GPIO_FSEL0_05_INPUT			(0 << 15)
#define RPI_GPIO_FSEL0_05_OUTPUT		(1 << 15)
	
#define RPI_GPIO_FSEL0_06_INPUT			(0 << 18)
#define RPI_GPIO_FSEL0_06_OUTPUT		(1 << 18)
	
#define RPI_GPIO_FSEL0_07_INPUT			(0 << 21)
#define RPI_GPIO_FSEL0_07_OUTPUT		(1 << 21)
	
#define RPI_GPIO_FSEL0_08_INPUT			(0 << 24)
#define RPI_GPIO_FSEL0_08_OUTPUT		(1 << 24)
	
#define RPI_GPIO_FSEL0_09_INPUT			(0 << 27)
#define RPI_GPIO_FSEL0_09_OUTPUT		(1 << 27)
	
	
	
	
/** The GPIO Peripheral is described in section 6 of the BCM2835 Peripherals
documentation.
	
There are 54 general-purpose I/O (GPIO) lines split into two banks. All
GPIO pins have at least two alternative functions within BCM. The
alternate functions are usually peripheral IO and a single peripheral
may appear in each bank to allow flexibility on the choice of IO voltage.
Details of alternative functions are given in section 6.2. Alternative
Function Assignments.
	
The GPIO peripheral has three dedicated interrupt lines. These lines are
triggered by the setting of bits in the event detect status register. Each
bank has its’ own interrupt line with the third line shared between all
bits.
	
The Alternate function table also has the pull state (pull-up/pull-down)
which is applied after a power down.
*/
typedef struct
{
	rpi_reg_rw_t		GPFSEL0;
	rpi_reg_rw_t		GPFSEL1;
	rpi_reg_rw_t		GPFSEL2;
	rpi_reg_rw_t		GPFSEL3;
	rpi_reg_rw_t		GPFSEL4;
	rpi_reg_rw_t		GPFSEL5;
	rpi_reg_ro_t		Reserved0;
	rpi_reg_wo_t		GPSET0;
	rpi_reg_wo_t		GPSET1;
	rpi_reg_ro_t		Reserved1;
	rpi_reg_wo_t		GPCLR0;
	rpi_reg_wo_t		GPCLR1;
	rpi_reg_ro_t		Reserved2;
	rpi_reg_wo_t		GPLEV0;
	rpi_reg_wo_t		GPLEV1;
	rpi_reg_ro_t		Reserved3;
	rpi_reg_wo_t		GPEDS0;
	rpi_reg_wo_t		GPEDS1;
	rpi_reg_ro_t		Reserved4;
	rpi_reg_wo_t		GPREN0;
	rpi_reg_wo_t		GPREN1;
	rpi_reg_ro_t		Reserved5;
	rpi_reg_wo_t		GPFEN0;
	rpi_reg_wo_t		GPFEN1;
	rpi_reg_ro_t		Reserved6;
	rpi_reg_wo_t		GPHEN0;
	rpi_reg_wo_t		GPHEN1;
	rpi_reg_ro_t		Reserved7;
	rpi_reg_wo_t		GPLEN0;
	rpi_reg_wo_t		GPLEN1;
	rpi_reg_ro_t		Reserved8;
	rpi_reg_wo_t		GPAREN0;
	rpi_reg_wo_t		GPAREN1;
	rpi_reg_ro_t		Reserved9;
	rpi_reg_wo_t		GPAFEN0;
	rpi_reg_wo_t		GPAFEN1;
	rpi_reg_ro_t		Reserved10;
	rpi_reg_wo_t		GPPUD;
	rpi_reg_wo_t		GPPUDCLK0;
	rpi_reg_wo_t		GPPUDCLK1;
	rpi_reg_ro_t		Reserved11;
}  rpi_gpio_t;
	
	
/*
 *  RPI_GetGpio      return pointer to GPIO register set
 *
 *  This routine returns a pointer to the GPIO register set defined in rpi_gpio_t
 *  above.  Changes to the fields within this register set are immediately written
 *  to the GPIO registers.
 */
	extern  rpi_gpio_t			*RPI_GetGpio(void);


/*
 *  RPI_GpioInit      initialize the GPIO register subsystem
 */
	extern void					RPI_GpioInit(void);
	

/*  ===============  Interrupts  =============  */

/** @brief See Section 7.5 of the BCM2836 ARM Peripherals documentation, the base
	address of the controller is actually xxxxB000, but there is a 0x200 offset
	to the first addressable register for the interrupt controller, so offset the
	base to the first register
*/
#define RPI_INTERRUPT_CONTROLLER_BASE ( PERIPHERAL_BASE + 0xB200 )
	
/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
See the BCM2835 ARM Peripherals manual, section 7.5 */
#define RPI_BASIC_ARM_TIMER_IRQ				(1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ			(1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ		(1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ		(1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ			(1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ			(1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ		(1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ		(1 << 7)
	
	
/** @brief The interrupt controller memory mapped register set */
typedef struct
{
	volatile uint32_t		IRQ_basic_pending;
	volatile uint32_t		IRQ_pending_1;
	volatile uint32_t		IRQ_pending_2;
	volatile uint32_t		FIQ_control;
	volatile uint32_t		Enable_IRQs_1;
	volatile uint32_t		Enable_IRQs_2;
	volatile uint32_t		Enable_Basic_IRQs;
	volatile uint32_t		Disable_IRQs_1;
	volatile uint32_t		Disable_IRQs_2;
	volatile uint32_t		Disable_Basic_IRQs;
} rpi_irq_controller_t;
	

/*
 *  RPI_GetIrqController      access register set for IRQ controller
 *
 *  This routine returns a pointer to the register set for the interrupt
 *  controller subsystem.  Changes to the fields in this register set
 *  are immediatly written to the IRQ controller registers.
 */
	extern rpi_irq_controller_t		*RPI_GetIrqController(void);
	

#endif
