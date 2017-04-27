#ifndef chunk_h_INCLUDED
#define chunk_h_INCLUDED

struct chunk{
	uint32_t start;
	size_t length;
    struct chunk *next;
};

int contains_chunk(uint32_t start, uint32_t length, struct chunk *list);

uint32_t get_chunk(uint32_t start, uint32_t length);

uint32_t get_any_chunk(uint32_t length);
#endif // chunk_h_INCLUDED

