#ifndef RPI_MAILBOX_H
#define RIP_MAILBOX_H

#include "rpi-base.h"
#include <stdint.h>

#define RPI_MAILBOX0_BASE (PERIPHERAL_BASE + 0xB880)

typedef enum {
	MB0_POWER_MANAGEMENT = 0,
	MB0_FRAMEBUFFER,
	MB0_VIRTUAL_UART,
	MB0_VCHIQ,
	MB0_LEDS,
	MB0_BUTTONS,
	MB0_TOUCHSCREEN,
	MB0_UNUSED,
	MB0_TAGS_ARM_TO_VC,
	MB0_TAGS_VC_TO_ARM,
} mailbox0_channel_t;

enum mailbox_status_reg_bits{
	ARM_MS_FULL = 0x80000000,
	ARM_MS_EMPTY = 0x40000000,
	ARM_MS_LEVEL = 0x400000FF,
};

typedef struct{
	volatile unsigned int Read;
	volatile unsigned int reserved1[((0x90 - 0x80 ) /4 )- 1];
	volatile unsigned int Poll;
	volatile unsigned int Sender;
	volatile unsigned int Status;
	volatile unsigned int Configuration;
	volatile unsigned int Write;
} mailbox_t;

extern void RPI_Mailbox0Write(mailbox0_channel_t channel, int value);
extern int RPI_Mailbox0Read(mailbox0_channel_t channel);

#endif
