#include <stdio.h>
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
#define SCREEN_DEPTH    16 

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags){
	volatile unsigned char *fb = NULL;
	int width = 800;
	int height = 600;
	int pitch = 0;
	int bpp = 16;

	console_init();

	RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;
	/*Enable the timer interrupt IRQ*/
	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

	/*Timer frequency = Clk/256 * 0x400*/
	RPI_GetArmTimer()->Load = 0x400;

	/* Setup the ARM Timer*/
	RPI_GetArmTimer()->Control = RPI_ARMTIMER_CTRL_23BIT | RPI_ARMTIMER_CTRL_ENABLE | RPI_ARMTIMER_CTRL_INT_ENABLE | RPI_ARMTIMER_CTRL_PRESCALE_256;

	chario_OutRegister((outcharFuncPtr)&console_putc);
	
	_enable_interrupts();
	
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

    if( mp ){
        printf( "Set ARM Clock Rate: %dHz\r\n", mp->data.buffer_32[1] );
	}else{
        printf( "Set ARM Clock Rate: NULL\r\n" );
    }
	RPI_WaitMicroSeconds(1000000);
	int i =0;
	char* String = "Booting OS";
	printf("\n%s\r\n", String);
	RPI_WaitMicroSeconds(1000000);
	for(i; i < 11; i++){
		printf("\n%d Percent complete\r\n", (i*10));
		RPI_WaitMicroSeconds(750000);
	}
	char* String2 = "Initializing Keyboard";
	printf("\n%s\r\n",String2); 
	UsbInitialise();
	u32 address = 0;
	if(KeyboardCount()){
		address = KeyboardGetAddress(0);
	}else{
		RPI_WaitMicroSeconds(500000);
		String2 = "No Keyboards Present";
		printf("\n%s\r\n", String2);
	}
	if(address){
		struct KeyboardLeds leds = KeyboardGetLedSupport(address);
		leds.NumberLock = 1;
		leds.CapsLock = 1;
		KeyboardSetLeds(address, leds);
	}
	String2 = "Booting OS complete";
	printf("\n%s\r\n", String2);
	RPI_WaitMicroSeconds(1000000);
	String2 = "I lied. This is the OS";
	printf("\n%s\r\n", String2);
	RPI_WaitMicroSeconds(2000000);
	String2 = "Booting Matrix instead";
	printf("\r\n%s\r\n\n\n", String2);
	RPI_WaitMicroSeconds(2000000);
	i = 0;
	while(1){
		printf("%d", i);
		if(i == 0){
			i = 1;
		}else{
			i = 0;
		}
	}
}
