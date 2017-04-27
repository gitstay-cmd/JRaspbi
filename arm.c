#include "gpio.h"
#include "atags.h"
#include "fb.h"
#include "ext.h"
#include "systimer.h"
#include "stdio.h"
#include "keyboard.h"
#include <usbd/usbd.h>
#include <device/hid/keyboard.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

extern fb_t fb;

struct fs_t *fs;

int place = 0;

void ls(const char *s){
    list_files();
}

struct command {
    char name[12];
    void (*command)(char *);
};

struct command *command_s[81];

void getc(char *args){
    //char c;    
    //printf("%c", c);
}

void assign_command(char *name, void (*command)(char *)){

    strcpy(command_s[place]->name, name);
    command_s[place++]->command = command;

}

struct command *get_command(const char *command_name){

	for(int i =0; i < 81; i++){
		if(command_s[i] == 0){
			break;
		}
		if(!strcmp(command_s[i]->name, command_name)){
    		return command_s[i];
		}
	}

	return (struct command*)0;

}

int handle_command(){
	return 1;
	
}

void empty_command(char *comm, int size){

    for(int i = 0; i < size; i++){
		comm[i] = 0;        
    }

}
void handle_input(){

    char c;

	char comm[81];
	printf(">>");
	do{
		KeyboardUpdate();
		c = KeyboardGetChar();
	} while(c != '\n');

	char *token = strtok(comm, " ");

	struct command *comman = get_command(token);

	comman->command((char *)&comm[(sizeof(token)-1)]);

	empty_command(comm, 81);

	printf("\n");
	
}

int main(uint32_t r0, uint32_t r1, uint32_t atags){

	ext2_init(&fs);


	
	setPinOn(16);

	fb_init();

	mp = RPI_PropertyGet(TAG_ALLOCATE_BUFFER);

	clear();
	int count;
	Result res;
	UsbInitialise();
	do {
    	UsbCheckForChange();
	} while (!(count = KeyboardCount()));

	/*res = KeyboardGetAddress(0);
	if(res){
    	struct KeyboardLeds leds = KeyboardGetLedSupport(res);
    	leds.NumberLock = 1;
    	leds.CapsLock = 1;
    	KeyboardSetLeds(res, leds);
	}*/

	list_files();

		
	while(1){
    	printf(">>Hello");
		handle_input();
    }
    return 1;
}

void exit(int code){

	while(1);

}
