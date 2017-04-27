#include <stddef.h>
#include <stdint.h>

extern int __bss_start__;
extern int __bss_end__;

extern void main(uint32_t r0, uint32_t r1, uint32_t atags);

void _cstartup(uint32_t r0, uint32_t r1, uint32_t r2){

	int *bss = &__bss_start__;
	int *bss_end = &__bss_end__;

	while (bss < bss_end)
    	*bss++ = 0;

    main( r0, r1, r2);

    while(1){}

}
