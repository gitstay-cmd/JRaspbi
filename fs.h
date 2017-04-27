#ifndef fs_h_INCLUDED
#define fs_h_INCLUDED
#include "ext.h"
#include "block.h"

typedef struct ext2_file FILE;

struct fs_t{
	struct block_t *parent;
	const char *fs_name;
	size_t block_size;

	FILE *(*fopen) (struct fs_t *, const char *path);
	size_t (*fread)(struct fs_t *, void *ptr, size_t byte_size, FILE *stream);
	size_t (*fwrite)(struct fs_t *, void *ptr, size_t byte_size, FILE *stream);
	int (*fclose) (struct fs_t *, FILE *fp);
	long (*fsize)(FILE *fp);
	int (*fseek)(FILE *fp, int offset, int whence);
	int (*fflush)(FILE *fp);
};


#endif // fs_h_INCLUDED

