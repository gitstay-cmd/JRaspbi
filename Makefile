CC=arm-none-eabi-gcc
CFLAGS= -O0 -mfpu=vfp -Wall -std=gnu99 -Icsud-master/include -fshort-wchar -ffreestanding -Wl,-T,rpi.x -nostartfiles -mfloat-abi=soft -march=armv6zk -mtune=arm1176jzf-s -g
TARGET= arm.c ext.c stdio.c codes.s atags.c keyboard.c chunk.c ramdisk.c block.c arm-timer.c fb.c font.c terminal.c libcsud.a interrupts.c gpio.c mailbox.c rpi-mailbox-interface.c uart.c systimer.c arms.S cstubs.c armc-cstartup.c mmio.c

all:
	$(CC) $(CFLAGS) $(TARGET) -o kernel.elf
	arm-none-eabi-objcopy kernel.elf -O binary kernel.img

clean:
	rm kernel.elf kernel.img
