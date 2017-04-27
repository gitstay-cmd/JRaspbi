#ifndef block_h_INCLUDED
#define block_h_INCLUDED
#include <stdint.h>
#include <stddef.h>
#include "fs.h"

struct block_t
{
	char *driver_name;
	char *device_name;
	uint8_t *device_id;
	size_t dev_id_len;
	int supports_multiple_block_read;
	int supports_multiple_block_write;

	int (*read)(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t block_num);
	int (*write)(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t block_num);
	size_t block_size;
	size_t num_blocks;

	struct fs_t *fs;
};

size_t block_read(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block);
size_t block_write(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block);
#endif // block_h_INCLUDED

