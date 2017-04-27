#include <stdlib.h>
#include <stdint.h>
#include "chunk.h"


struct chunk *free_b = (void*)0;
struct chunk *used = (void*)0;

uint32_t max_free = 0;

void chunk_add(uint32_t start, uint32_t length, struct chunk **list){
	struct chunk *new = (struct chunk*)malloc(sizeof(struct chunk));
	new->start = start;
	new->length = length;
	new->next = *list;
	*list = new;
}

int chunk_overlaps(uint32_t start, uint32_t length, struct chunk *list){
	
  	while(list){
      	uint32_t l_start = list->start;
  		uint32_t l_end = list->start + list->length;
  		uint32_t end = start + length;

  		if((start < l_start) && (end > l_end)){
      		return 1;
  		}
      	list = list->next; 
  	}
    return 0;
   	
}

int chunk_contains(uint32_t start, uint32_t length, struct chunk *list){

	while(list){
		uint32_t l_start = list->start;
		uint32_t l_end = list->start + list->length;
		uint32_t end = start + length;

		if((start >= l_start) && (end <= l_end)){
    		return 1;
		}
		list = list->next;
	}
	return 0;

}

int can_allocate_chunk(uint32_t start, uint32_t length){
	if(!chunk_contains(start, length, free_b))
    	return 0;
    if(chunk_overlaps(start, length, used))
        return 0;
    return 1;
}

uint32_t get_chunk(uint32_t start, uint32_t length){

	if(can_allocate_chunk(start, length)){

    	chunk_add(start, length, &used);
    	return start;
	}
	return 0;
}

uint32_t get_any_chunk(uint32_t length){

    uint32_t addr = 0;
    while( addr > max_free){

        if(can_allocate_chunk(addr, length)){
            return get_chunk(addr, length);
        }
        addr += 0x1000;
    }

    return 0;

}
