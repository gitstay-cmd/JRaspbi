#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include "usbd/usbd.h"
#include "device/hid/keyboard.h"
#include "arm-timer.h"
#include "rpi-interrupts.h"
#include "rpi-gpio.h"
#include "timer.h"
#include "rpi-mailbox-interface.h"
#include "rpi-mailbox.h"
#include "rpi-aux.h"
#include "console.h"
#include "printf.h"
#include "chario.h"

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define SCREEN_DEPTH    16      /* 16 or 32-bit */

#define COLOUR_DELTA    0.05    /* Float from 0 to 1 incremented by this amount */

/** Main function - we'll never return from here */
void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags){
	volatile unsigned char* fb = NULL;
	int width = SCREEN_WIDTH, height = SCREEN_HEIGHT, bpp = SCREEN_DEPTH;
	int pitch = 0;
	int index;
    /* Assign the address of the GPIO peripheral (Using ARM Physical Address) */
		RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;
	/*Enable the timer interrupt IRQ*/
	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

	/*Timer frequency = Clk/256 * 0x400*/
	RPI_GetArmTimer()->Load = 0x400;

	/* Setup the ARM Timer*/
	RPI_GetArmTimer()->Control = RPI_ARMTIMER_CTRL_23BIT | RPI_ARMTIMER_CTRL_ENABLE | RPI_ARMTIMER_CTRL_INT_ENABLE | RPI_ARMTIMER_CTRL_PRESCALE_256;
	
	
	UsbInitialise();
	u32 address = 0;
	if(KeyboardCount()){
		address = KeyboardGetAddress(0);
	}
	
	console_init();
		
	chario_OutRegister((outcharFuncPtr)&console_putc);
	if(address){
		chario_InRegister((incharFuncPtr)KeyboardGetKeyDown);
		struct KeyboardLeds leds = KeyboardGetLedSupport(address);
		leds.NumberLock = 1;
		leds.CapsLock = 1;
		KeyboardSetLeds(address, leds);
	}
	printf("Set");
	RPI_PropertyInit();
    RPI_PropertyAddTag( TAG_GET_BOARD_MODEL );
    RPI_PropertyAddTag( TAG_GET_BOARD_REVISION );
    RPI_PropertyAddTag( TAG_GET_FIRMWARE_VERSION );
    RPI_PropertyAddTag( TAG_GET_BOARD_MAC_ADDRESS );
    RPI_PropertyAddTag( TAG_GET_BOARD_SERIAL );
    RPI_PropertyAddTag( TAG_GET_MAX_CLOCK_RATE, TAG_CLOCK_ARM );
    RPI_PropertyProcess();

    rpi_mailbox_property_t* mp;
    mp = RPI_PropertyGet( TAG_GET_BOARD_MODEL );

    if( mp )
        printf( "Board Model: %d\r\n", mp->data.value_32 );
    else
        printf( "Board Model: NULL\r\n" );

    mp = RPI_PropertyGet( TAG_GET_BOARD_REVISION );

    if( mp )
        printf( "Board Revision: %d\r\n", mp->data.value_32 );
    else
        printf( "Board Revision: NULL\r\n" );

    mp = RPI_PropertyGet( TAG_GET_FIRMWARE_VERSION );

    if( mp )
        printf( "Firmware Version: %d\r\n", mp->data.value_32 );
    else
        printf( "Firmware Version: NULL\r\n" );

    mp = RPI_PropertyGet( TAG_GET_BOARD_MAC_ADDRESS );

    if( mp )
        printf( "MAC Address: %2.2X:%2.2X:%2.2X:%2.2X:%2.2X:%2.2X\r\n",
               mp->data.buffer_8[0], mp->data.buffer_8[1], mp->data.buffer_8[2],
               mp->data.buffer_8[3], mp->data.buffer_8[4], mp->data.buffer_8[5] );
    else
        printf( "MAC Address: NULL\r\n" );

    mp = RPI_PropertyGet( TAG_GET_BOARD_SERIAL );

    if( mp )
        printf( "Serial Number: %8.8X%8.8X\r\n",
                mp->data.buffer_32[0], mp->data.buffer_32[1] );
    else
        printf( "Serial Number: NULL\r\n" );

    mp = RPI_PropertyGet( TAG_GET_MAX_CLOCK_RATE );

    if( mp )
        printf( "Maximum ARM Clock Rate: %dHz\r\n", mp->data.buffer_32[1] );
    else
        printf( "Maximum ARM Clock Rate: NULL\r\n" );

    /* Ensure the ARM is running at it's maximum rate */
    RPI_PropertyInit();
    RPI_PropertyAddTag( TAG_SET_CLOCK_RATE, TAG_CLOCK_ARM, mp->data.buffer_32[1] );
    RPI_PropertyProcess();

    RPI_PropertyInit();
    RPI_PropertyAddTag( TAG_GET_CLOCK_RATE, TAG_CLOCK_ARM );
    RPI_PropertyProcess();

    mp = RPI_PropertyGet( TAG_GET_CLOCK_RATE );

    if( mp )
        printf( "Set ARM Clock Rate: %dHz\r\n", mp->data.buffer_32[1] );
    else
        printf( "Set ARM Clock Rate: NULL\r\n" );

    /* Initialise a framebuffer... */
    RPI_PropertyInit();
    RPI_PropertyAddTag( TAG_ALLOCATE_BUFFER );
    RPI_PropertyAddTag( TAG_SET_PHYSICAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT );
    RPI_PropertyAddTag( TAG_SET_VIRTUAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT * 2 );
    RPI_PropertyAddTag( TAG_SET_DEPTH, SCREEN_DEPTH );
    RPI_PropertyAddTag( TAG_GET_PITCH );
    RPI_PropertyAddTag( TAG_GET_PHYSICAL_SIZE );
    RPI_PropertyAddTag( TAG_GET_DEPTH );
    RPI_PropertyProcess();

    if( ( mp = RPI_PropertyGet( TAG_GET_PHYSICAL_SIZE ) ) )
    {
        width = mp->data.buffer_32[0];
        height = mp->data.buffer_32[1];

        printf( "Initialised Framebuffer: %dx%d ", width, height );
    }

    if( ( mp = RPI_PropertyGet( TAG_GET_DEPTH ) ) )
    {
        bpp = mp->data.buffer_32[0];
        printf( "%dbpp\r\n", bpp );
    }

    if( ( mp = RPI_PropertyGet( TAG_GET_PITCH ) ) )
    {
        pitch = mp->data.buffer_32[0];
        printf( "Pitch: %d bytes\r\n", pitch );
    }

    if( ( mp = RPI_PropertyGet( TAG_ALLOCATE_BUFFER ) ) )
    {
        fb = (unsigned char*)mp->data.buffer_32[0];
        printf( "Framebuffer address: %8.8X\r\n", (unsigned int)fb );
    }
	printf("Insert OS Boot Screen here\n Boot:<================>\n Non-existent Filesystem: Loaded\n");
	RPI_WaitMicroSeconds(50000);
	_enable_interrupts();
	while(1){
	}
}

void exit(int code){
	while(1);
}
