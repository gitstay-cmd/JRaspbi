#include <stdint.h>

#include "gpio.h"
#include "mailbox.h"

mailbox_t* mailbox = (mailbox_t*)Rpi_mailbox0_base;

void Rpi_MailboxWrite(mailbox_channel channel, unsigned int value){

	value &= ~(0xF);
	value |= channel;
	
	while((mailbox->STATUS & ARM_MS_FULL) != 0){}

	mailbox->WRITE = value;

}

unsigned int Rpi_MailboxRead(mailbox_channel channel){

	unsigned int value;
	
	while((mailbox->STATUS & ARM_MS_EMPTY) != 0 || ((value = mailbox->READ) & 0xF) != channel){}

	return (value >> 4);
}
