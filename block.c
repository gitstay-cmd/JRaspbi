#include <stdint.h>
#include "block.h"

size_t block_read(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block){

	uint32_t block_offset = 0;

	int buf_offset = 0;

	if(dev->supports_multiple_block_read){

    	return dev->read(dev, buf, buf_size,starting_block);
	}
	
	do {
    	int read;

    	size_t to_read = buf_size;

    	if(buf_size > dev->block_size){
        	to_read = dev->block_size;
    	}
				
    	int tries = 0;

    	while(1){

        	read = dev->read(dev, &buf[buf_offset], to_read, starting_block + block_offset);

        	if (read < 0){

            	tries++;

            	if( tries > 12){

                	return read;

            	}

        	}

    	}
    	block_offset++;
    	
    	buf_offset += to_read;
    	
      	if(buf_size < dev->block_size){
          	
          	buf_size = 0;

      	}else{

			buf_size -= to_read;

      	}
      	
	}while(buf_size > 0);

	return (size_t)buf_offset;
}

size_t block_write(struct block_t *dev, uint8_t *buf, size_t buf_size, uint32_t starting_block){

   	int block_offset = 0;

   	uint32_t buff_offset =0 ;

   	size_t to_write = buf_size;

    if(buf_size > dev->block_size){

		to_write = dev->block_size;
   	}

	if(dev->supports_multiple_block_write){
    	dev->write(dev, buf, buf_size, starting_block);
	}

	int write;
	do{

    	int tries = 0;
    	while(1){

        	write = dev->write(dev, &buf[buff_offset], to_write, starting_block + block_offset);

			if(write > 0){

        		tries++;

        		if(tries > 12){

            		return write;

        		}
        		write = dev->write(dev, &buf[buff_offset], to_write, starting_block + block_offset);
    		}
    	}

		block_offset++;

		buff_offset += to_write;
	
		if(buf_size < dev->block_size){

    		buf_size = 0;

		}else buf_size -= to_write;

	}while(buf_size > 0);
	
   	return (size_t)buff_offset;
}
