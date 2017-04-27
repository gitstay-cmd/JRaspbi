#include "gpio.h"
#include "mmio.h"
#include <stddef.h>
#include <stdint.h>

void SetPinFunction(gpio_pin gpio, gpio_function function){

	switch(gpio / 10){
		case 0:
    		mmio_write(GPIO_BASE + GPFSEL0, ((function) << ((gpio%10)*3)));
    		break;
    	case 1:
        	mmio_write(GPIO_BASE + GPFSEL1, ((function) << ((gpio%10)*3)));
        	break;
        case 2:
            mmio_write(GPIO_BASE + GPFSEL2, ((function) << ((gpio%10)*3)));
            break;
        case 3:
            mmio_write(GPIO_BASE + GPFSEL3, ((function) << ((gpio%10)*3)));
            break;
        case 4:
            mmio_write(GPIO_BASE + GPFSEL4, ((function) << ((gpio%10)*3)));
            break;
        case 5:
            mmio_write(GPIO_BASE + GPFSEL5, ((function) << ((gpio%10)*3)));
            break;
        default:
            break;
	}

}

void SetGpioOutPut(gpio_pin gpio){

	SetPinFunction(gpio, GPIO_OUTPUT);

}

void SetGpioInput(gpio_pin gpio){

	SetPinFunction(gpio, GPIO_INPUT);

}

uint32_t getPinValue(gpio_pin gpio){

	uint32_t command;

	gpio_value result = UNKOWN;

	switch(gpio/32){
    	case 0:
        	command = GPLEV0;
        	break;
        case 1:
            command = GPLEV1;
            break;
        default:
            break;
	}

	result = mmio_read(GPIO_BASE + command) & (1 << (gpio%32));

	if(result != UNKOWN){
    	if(result){
        	result = HI;
    	}
	}
	return result;
}

void PinToggle(gpio_pin gpio){
    if(getPinValue(gpio)){
        setPinOff(gpio);
    }else{
        setPinOn(gpio);
    }
}

void setPinOn(gpio_pin gpio){

	uint32_t command;

	if(gpio/32){

    	command = GPFSET1;
    	
	}else{
    	
    	command = GPFSET0;
    	
	}

	mmio_write(GPIO_BASE + command, (1 << (gpio % 32)));

}

void setPinOff(gpio_pin gpio){

	uint32_t command;
	
	if(gpio/32){
    	
		command = GPCLR1;
		
	}else{
    	
		command = GPCLR0;
		
	}
	
	mmio_write(GPIO_BASE + command, (1 << (gpio % 32))); 

}

