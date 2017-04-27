#ifndef mailbox_h_INCLUDED
#define mailbox_h_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "bcm2835.h"

#define Rpi_mailbox0_base (Peripheral_Base + 0xB880)

typedef enum{

	MB0_POWER_MANAGEMENT =0,
	MB0_FRAMEBUFFER,
	MB0_VIRTUAL_UART,
	MB0_VCHIQ,
	MB0_LEDS,
	MB0_BUTTONS,
	MB0_TOUCHSCREEN,
	MB0_UNUSED,
	MB0_TAGS_ARM_TO_VC,
	MB0_TAGS_VC_TO_ARM,
} mailbox_channel;

enum mailbox_status{
    ARM_MS_FULL = 0x80000000,
    ARM_MS_EMPTY = 0x40000000,
    ARM_MS_LEVEL = 0x400000FF,
};

typedef struct {
    volatile uint32_t		READ;
    volatile uint32_t		RES1;
    volatile uint32_t		RES2;
    volatile uint32_t		RES3;
    volatile uint32_t		PEEK;
    volatile uint32_t		SENDER;
    volatile uint32_t		STATUS;
    volatile uint32_t 		CONFIG;
    volatile uint32_t		WRITE;
}mailbox_t;

void Rpi_MailboxWrite(mailbox_channel channel, unsigned int value);

unsigned int Rpi_MailboxRead(mailbox_channel channel);
#endif // mailbox_h_INCLUDED
