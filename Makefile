OBJS = uspilibrary.o dwhcidevice.o dwhciregister.o dwhcixferstagedata.o usbconfigparser.o usbdevice.o usbdevicefactory.o usbendpoint.o usbrequest.o usbstandardhub.o devicenameservice.o macaddress.o smsc951x.o string.o util.o usbmassdevice.o dwhciframeschednper.o dwhciframeschedper.o keymap.o usbkeyboard.o dwhcirootport.o usbmouse.o dwhciframeschednsplit.o usbgamepad.o synchronize.o usbstring.o

all:
	arm-none-eabi-gcc -nostartfiles -I ../../uspi-master/env/include -fshort-wchar -O2 -mfpu=vfp -mfloat-abi=hard -mfloat-abi=softfp -mtune=arm1176jzf-s -Wl,-T,rpi.x rpi-mailbox-interface.c rpi-uart.c rpi-aux.c rpi-mailbox.c arm-timer.c rpi-gpio.c rpi-interrupts.c timer.c arm.c cstartup.c cstubs.c start.S -o kernel.elf
	arm-none-eabi-objcopy kernel.elf -O binary kernel.img	
test:
	arm-none-eabi-gcc -nostartfiles -mfpu=vfp -I../../csud-master/include/ -I/uspi/ -fshort-wchar -O2 -mfloat-abi=hard -mfloat-abi=softfp -mtune=arm1176jzf-s -Wl,-T,rpi.x -Wl,--no-wchar-size-warning timer.c font.c chario.c console.c mailbox.c printf.c rpi-interrupts.c rpi-mailbox.c arm-timer.c rpi-gpio.c rpi-mailbox-interface.c rpi-uart.c rpi-aux.c  arm2.c cstartup.c cstubs.c start.S libcsud.a -o kernel.elf
	arm-none-eabi-objcopy kernel.elf -O binary kernel.img
move:
	rm /boot/kernel.img
	mv kernel.img /boot/kernel.img

clean:
	rm *.img *.elf
