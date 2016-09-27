#ifndef RPI_BASE_H
#define RPI_BASE_H

#include <stdint.h>

#ifdef RPI2
	#define PERIPHERAL_BASE 0x3F000000UL
#else
	#define PERIPHERAL_BASE	0x20000000UL
#endif

typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;

#endif
