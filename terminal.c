#include "terminal.h"
#include "fb.h"

void terminal_prints(const char *string){
	char c;
	while((c = *string++))
    	terminal_printc((int)c);
}

void terminal_clear(){
    clear();
}

void terminal_printc(const int ch){
    console_drawc(ch);
}
