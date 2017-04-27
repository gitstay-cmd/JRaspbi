#include <device/hid/keyboard.h>
#include <usbd/usbd.h>
#include <stdint.h>
#include "stdio.h"

extern const char KeysNormal[];

uint32_t address = 0;

uint8_t keys[6];

uint32_t get_address(){
    return address;
}

void LogPrint(char* message, u32 messageLength){
    printf(message);
}

void KeyboardUpdate(){
    
	if(address == 0){
        PollFailed: UsbCheckForChange();
		int count = KeyboardCount();
		if(count == 0){
    		address = 0;
    		return;
		}

		address = KeyboardGetAddress(0);
		if(address == 0){
    		return;
		}
		for(int i = 0; i < 6; i++){
    		keys[i] = KeyboardGetKeyDown(address, 0);
		}
    	if(KeyboardPoll(address)){
        	goto PollFailed;
    	}
	}else{
    	
    	for(int i =0; i < 6; i++){
        	keys[i] = KeyboardGetKeyDown(address,0);
    	}
    	if(KeyboardPoll(address)){
        	goto PollFailed;
    	}

	}
}

uint8_t KeyWasDown(uint8_t scan_code){
  	for(int i =0; i < 6; i++){
      	if(keys[i] == scan_code)
          	return 1;
  	}
  	return 0;
}

char KeyboardGetChar(){
    if(address == 0){
        return 0;
    }
    for(int i =0; i < 6; i++){

        if(keys[i] == 0){
      		break;
      		
        }
        if(!KeyWasDown(keys[i])){
            uint8_t j = keys[i];
            while(KeysNormal[j++] == 0){
                
            }
            return KeysNormal[j];
            printf("%c\n", KeysNormal[j]);
        }

    }
    return 0;
}

