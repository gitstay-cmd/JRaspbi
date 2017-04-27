#include <stddef.h>
#include <stdint.h>

#ifndef mmio_h_INCLUDED
#define mmio_h_INCLUDED

void mmio_write(uint32_t reg, uint32_t data);

uint32_t mmio_read(uint32_t reg);

void delay(int32_t count);

#endif // mmio_h_INCLUDED

