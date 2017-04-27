#ifndef ext_h_INCLUDED
#define ext_h_INCLUDED
#include <stdint.h>
#include <stddef.h>
#include "fs.h"

#define MAX_FILES 12
#define MAX_BLOCKS 60
#define MAX_SIZE MAX_BLOCKS * 512
#define MAX_CHILDREN 3

struct ext2_inode{
    uint32_t starting_address;
    uint32_t cur_block;
    uint32_t size;
};

/*struct dirent{
    char *name;
    struct ext2_file *file;
    uint32_t start_address;
    uint32_t size;
    struct dirent *parent;
    struct dirent *children[MAX_CHILDREN];
};*/

/*struct ext2_fs{
    struct fs_t fs;
    uint32_t no_files;
    struct dirent *root;
    struct dirent *cur_dir;
};*/

struct ext2_file {
    char *name;
    uint32_t start_address;
    uint32_t size;
    uint32_t pos;
    struct fs_t *fs;
    struct ext2_inode *in;
};

typedef struct ext2_file FILE;

/*struct dirent *traverse_directory(const char *path, struct dirent *root);
struct dirent *find_file(struct ext2_fs *fs, const char *path);*/
int f_fread(FILE *fp, uint8_t *buf, size_t buf_size);

int ext2_init(struct fs_t **fs);

size_t fread(struct fs_t *fs, void *ptr, size_t byte_size, FILE *stream);

char *fgets(FILE *fp);

void list_files();

char fgetc(FILE *fp);
#endif // ext_h_INCLUDED

