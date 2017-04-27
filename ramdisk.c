#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "fs.h"
#include "block.h"
#include "chunk.h"

#define BLOCK_SIZE 		512
static char *driver_name = "ramdisk";

int register_fs(struct block_t *dev, struct fs_t *fs);
static int ramdisk_write(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t block_no);
static int ramdisk_read(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t block_no);

struct ramdisk_dev{
    struct block_t block;
    uint32_t address;
    size_t size;
};

int ramdisk_init(uint32_t address, size_t size, char *name, struct fs_t *fs){

	if(!address){ 

    	address = (uint32_t)get_any_chunk((uint32_t)size);

	}
	if(!address){

    	return 0;

	}

	struct ramdisk_dev *dev = (struct ramdisk_dev *)malloc(sizeof(struct ramdisk_dev));

	if(dev == NULL){
    	return -1;
	}

	memset(dev, 0, sizeof(struct ramdisk_dev));

	dev->block.block_size = BLOCK_SIZE;
	dev->block.device_name = (char *)malloc(strlen(name) +1);

	if(dev->block.device_name == NULL){
    	return -1;
	}

	strcpy(dev->block.device_name, name);

	dev->address = address;

	dev->block.driver_name = driver_name;

	dev->block.supports_multiple_block_read = 1;
	dev->block.supports_multiple_block_write = 1;
	dev->block.read = ramdisk_read;
	dev->block.write = ramdisk_write;

	int ret = register_fs((struct block_t*)dev, fs);

	if(ret){
    	free(dev);
	}
	return ret;
	
}

int register_fs( struct block_t *dev, struct fs_t *fs){

	dev->fs = fs;
	fs->parent = dev;
	return 1;
}

static int ramdisk_write(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t block_no){
	struct ramdisk_dev *rdev = (struct ramdisk_dev *)dev;

	uint32_t start_address = rdev->address + (uint32_t)(dev->block_size * block_no);
	uint32_t end_address = start_address + buf_size;

	if(buf_size > end_address - start_address){
    	buf_size = (end_address - start_address);
	}

	memcpy((void*)start_address, buf, buf_size);

	return (int)buf_size;
}

static int ramdisk_read(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t block_no){
	struct ramdisk_dev *rdev = (struct ramdisk_dev *)dev;

	uint32_t start_address = rdev->address + (uint32_t)(dev->block_size * block_no);
	uint32_t end_address = start_address + buf_size;

	if(buf_size > end_address - start_address){
    	buf_size = (end_address - start_address);
	}

	memcpy(buf, (void*)start_address, buf_size);

	return (int)buf_size;
}
