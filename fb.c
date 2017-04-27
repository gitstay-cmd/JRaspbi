
#include <stdint.h>
#include "fb.h"

#define CHAR_W			8
#define CHAR_H			12

#define COLOR_DELTA	0.05

fb_t fb;

void fb_init(){

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_ALLOCATE_BUFFER);
	RPI_PropertyAddTag(TAG_SET_PHYSICAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT);
	RPI_PropertyAddTag(TAG_SET_VIRTUAL_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT * 2);
	RPI_PropertyAddTag(TAG_SET_DEPTH, SCREEN_DEPTH);
	RPI_PropertyAddTag(TAG_GET_PITCH);
	RPI_PropertyAddTag(TAG_GET_PHYSICAL_SIZE);
	RPI_PropertyAddTag(TAG_GET_DEPTH);
	RPI_PropertyProcess();

	mp = RPI_PropertyGet(TAG_ALLOCATE_BUFFER);
	fb.fb = mp->data.buffer_32[0];

	mp = RPI_PropertyGet(TAG_GET_PHYSICAL_SIZE);
	fb.width = mp->data.buffer_32[0];
	fb.height = mp->data.buffer_32[1];

	mp = RPI_PropertyGet(TAG_GET_PITCH);
	fb.pitch = mp->data.buffer_32[0];

	mp = RPI_PropertyGet(TAG_GET_DEPTH);
	fb.depth = mp->data.buffer_32[0];

	fb.x = 0;
	fb.y = 0;

	fb.fore = CONSOLE_WHITE;
	fb.back = CONSOLE_BLACK;

}

const uint32_t fb_address(){
    return fb.fb;
}

const uint32_t get_pitch(){
    return fb.pitch;
}

const uint32_t get_width(){
    return fb.width;
}

const uint32_t get_height(){
    return fb.height;
}

void draw_pixel(const int x, const int y, uint16_t color){
	uint16_t* addr = (uint16_t*)(fb.fb + (x * fb.depth >> 3) + (y * fb.pitch));
	*addr = color;

}

void console_puts(const char *str){
	char c;
	while((c = *str++)){
        console_drawc((int)c);
    }
}

void console_drawc(int c){

	switch(c){

		case (int)'\n':
			newline();
			break;
		case (int)'\t':
			fb.x += 4;
			break;
		case (int)'\r':
			fb.x = 0;
			break;
		case (int)'\b':
			if(fb.x){
    			fb.x--;
    			draw_char((int)' ', fb.x* CHAR_W, fb.y * CHAR_H, fb.fore, fb.back); 	
			}
			break;
		default:
    		draw_char(c, fb.x*CHAR_W, fb.y * CHAR_H, fb.fore, fb.back);
    		fb.x++;
    		if(fb.x == fb.width/CHAR_W){
        		newline();
    		}
    		break;
	}

}

void draw_char(const int c, const int x, int y, const uint16_t fore, const uint16_t back){
    
	int i, j;
	uint8_t line;
	int index = c *(int) CHAR_H;

	for(i = 0; i < CHAR_H; i++){

		line = (uint8_t) FONT[index++];
		
		for(j = 0; j < CHAR_W; j++){

			if(line & 0x1){draw_pixel(x+j, y+i, fore);}
			else draw_pixel(x+j, y+i, back);
			line >>=1;
		}

	}

}



void newline(){
    uint8_t* to;
    uint8_t* from;
	int i;
	fb.y++;
	fb.x = 0;

	if(fb.y == (fb.height/ CHAR_H)){

    	to = (uint8_t*)fb.fb;
    	from = to + (CHAR_H * fb.pitch);

	
		for(i = 0; i < ((fb.height - CHAR_H) * fb.pitch); i++){
    		*to++ = *from++;
		}

		uint16_t *addr = (uint16_t*)(fb.fb) + (fb.height - CHAR_H)* fb.width;

		for( i = 0; i < (CHAR_H * fb.width); i++){
    		*addr++ = fb.back;
		}

		fb.y--;
	}
}

void clear_line(const int line){
	int i;
	uint16_t* addr;
	if(line > fb.height / CHAR_H){fb.y = 0;}
	else{fb.y = line;}

	fb.x = 0;

	addr = (uint16_t*)(fb.fb + (line * CHAR_H * fb.depth * fb.width));
	for(i = 0; i < (CHAR_H * fb.width); i++){
    	*addr++ = fb.back;
	}

}

void clear(){

	uint16_t *addr = (uint16_t*)fb.fb;
	uint32_t i;

	for(i = 0; i <(fb.height * fb.width); i++){
		*addr++ = fb.back;
	}

	fb.x = 0;
	fb.y = 0;

}
