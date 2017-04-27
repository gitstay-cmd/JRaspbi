#include "rpi-mailbox-interface.h"
#include <stdio.h>

void Print_Memory(){

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_ARM_MEMORY);
	RPI_PropertyAddTag(TAG_GET_VC_MEMORY);
	RPI_PropertyProcess();

	rpi_mailbox_property_t* mp;

	mp = RPI_PropertyGet(TAG_GET_ARM_MEMORY);

	printf( "ARM Memory Location: %d\n ARM Memory Size: %d\n", mp->data.buffer_32[0], mp->data.buffer_32[1]);

	mp = RPI_PropertyGet(TAG_GET_ARM_MEMORY);

	printf( "VC Memory Location: %d\n VC Memory Size: %d\n", mp->data.buffer_32[0], mp->data.buffer_32[1]);
	
}
