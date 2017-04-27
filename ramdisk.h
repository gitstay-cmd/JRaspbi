#ifndef ramdisk_h_INCLUDED
#define ramdisk_h_INCLUDED
#include <stdint.h>

void ramdisk_init(uint32_t address, size_t size, char *name, struct fs_t *fs);
#endif // ramdisk_h_INCLUDED

