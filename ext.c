#include <stdint.h>
#include <stdlib.h>
#include "fs.h"
#include "ramdisk.h"
#include <string.h>
#include "ext.h"
#include "stdio.h"

int available_address[MAX_FILES] = {0, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360};

char *fs_name = "ext";

static FILE *files[MAX_FILES];

FILE *is_present(const char *name){

    for(int i =0; i < MAX_FILES; i++){

        if(files[MAX_FILES] != NULL){

 			if(!strcmp(files[MAX_FILES]->name, name)){
     			return files[MAX_FILES];		
 			}

        }

    }
    return (FILE *)0;

}

uint32_t get_start_address(){
	int addr;
    for(int i =0; i < MAX_FILES; i++){

        if(available_address[i] != -1){
            addr = available_address[i];
            available_address[i] = -1;
            return addr;
        }

    }
    return -1;
}

FILE *fopen(struct fs_t *fs, const char *name){
	FILE *fp;
	struct ext2_inode *in = malloc(sizeof(struct ext2_inode));
    if ((fp = is_present(name))){
        fp->in = in;
		fp->in->starting_address = fp->start_address;
        fp->fs = fs;
        fp->in->size = fp->size;
        fp->pos = 0;
    	return fp;
    }else{
        for(int i = 0; i < MAX_FILES; i++){

            if(files[i] == NULL){
                
               	fp = malloc(sizeof(FILE));

               	files[i] = fp;

               	fp->in = in;

               	fp->start_address = get_start_address();
				fp->name = (char*)malloc(sizeof(name));
				strcpy(fp->name, name);
               	if(fp->start_address == -1){
                   	return (FILE*)0;
               	}
               	
				fp->in->starting_address = fp->start_address;
				fp->fs = fs;
				fp->in->size = fp->size = 0;
				fp->pos = 0;
				return fp;
            }
        }
        free(in);
        return (FILE *)0;
    }
}

/*struct dirent *make_dirent(char *path){
	char *tokenized = (char*)malloc(sizeof(path));
	strcpy(tokenized, path);
	char *token = strtok(path, "/");
}*/

int fflush(FILE *fp){
    //to be implemented
    return 0;
}

int fseek(FILE *fp, int offset, int whence){

    return 0;
}

char fgetc(FILE *fp){
    char *c = malloc(sizeof(char));
    f_fread(fp, (uint8_t*)c, 1); 
    char ret = *c;
    free(c);
    return ret;
}

char *fgets(FILE *fp){
    char *str = malloc(sizeof(char)* 81);
    int i = 0; 
    while((str[i] = fgetc(fp)) != '\n' && i < 81){
        i++;
    }
    
    return str;
}

int f_fread(FILE *fp, uint8_t *buf, size_t buf_size){
    return fread(fp->fs, (void*)buf, sizeof(buf), fp);
}

size_t fread(struct fs_t *fs, void *ptr, size_t byte_size, FILE *stream){

    if(byte_size > stream->in->size){
        byte_size = stream->in->size;
    }
    
    fs->parent->read(fs->parent, (uint8_t*)ptr, byte_size , (stream->pos + stream->in->starting_address)/fs->block_size);    
   	stream->pos += byte_size;
   	return 1;
}

int fcreate(struct fs_t *fs, char *path){
    /*struct dirent *dir;*/
    return 1;
}
            
int fclose(struct fs_t *fs, FILE *fp){
    free(fp->in);
    return 0;
}

/*int unlink(struct dirent *dir){
    uint32_t start = dir->file->in->starting_address;
    free(dir->file->in);
    free(dir->file);
    free(dir);
    available_address[(starting_address/60) % 60] = starting_address;
}*/

int fsize(FILE *fp){
    return fp->in->size;
}

size_t fwrite(struct fs_t *fs, void *ptr, size_t byte_size, FILE *stream){

	if((byte_size + stream->pos) > MAX_SIZE){
    	byte_size = (MAX_SIZE - stream->pos);
	}

	fs->parent->write(fs->parent, (uint8_t*)ptr, byte_size, (stream->pos + stream->in->starting_address)/fs->block_size);
	stream->in->size += byte_size;
	return byte_size;
}

/*struct dirent *traverse_directory(const char *path, struct dirent *root){
	char *strpath = strdup(path);

    char *token = strtok(strpath, "/");
	
	while(token != NULL){

    	for(int i = 0; i < 3; i++){

        	if(root->children[i] == NULL){   
			}

        	else{

        		if(!strcmp(root->children[i]->name, token)){

            		root = root->children[i];

            		token = strtok(NULL, "/");

            		if(token == NULL){

                		return root;

            		}

        		}

        	}

    	}

	}

}*/

/*struct dirent *find_file(struct ext2_fs *fs, const char *path){
	if(path[0] == '/'){
    	return (struct dirent*)traverse_directory(&(path[1]),fs->root);
	}
	else{
    	return (struct dirent *)traverse_directory(path, fs->cur_dir); 
	}
}*/

int ext2_init(struct fs_t **fs){

	struct fs_t *ret = (struct fs_t *)malloc(sizeof(struct fs_t));

	memset(ret, 0, sizeof(struct fs_t));

	ret->fopen = fopen;
	ret->fread = fread;
	ret->fwrite = fwrite;
	ret->fclose = fclose;
	ret->fseek = fseek;
	ret->fs_name = fs_name;

	ramdisk_init(0, MAX_SIZE * MAX_FILES, "name", ret);

	*fs = (struct fs_t*)ret;
	
	return 1;
}

void list_files(){

    for(int i = 0; i < MAX_FILES; i++){

		if(files[MAX_FILES] != NULL){ 

    		printf("FILE: %s\n", files[MAX_FILES]->name);

		}
		
    }
}
