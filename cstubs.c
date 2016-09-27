#include<sys/times.h>
#include<sys/stat.h>
#include "rpi-aux.h"
#include<errno.h>
#undef errno
extern int errno;


char *__env[1] = {0};
char **environ = __env;
/*A helper function written in assembler to aidus in allocating memory */
extern caddr_t _get_stack_pointer(void);

void _exit(int status){
	(void)status;

	while(1){
		}
}

/*To be turned into a file system*/
int _close(int file){
	return -1;
}

/*Transfer control to a new process. Minimal implementation*/
int execve(char *name, char **argv, char **env){
	errno = ENOMEM;
	return -1;
}

/* Create a new process. Minimal implementation*/
int fork(void){
	errno = EAGAIN;
	return -1;
}

/*Status of open file. For consistency all files regarded as character special devices. The sys/stat.h is distributed in the include sub directory of this c library*/
int _fstat(int file, struct stat *st){
	st->st_mode = S_IFCHR;
	return 0;
}

/*Process-ID; this is sometimes used to generate strings unlikely to conflict with other processes. Minimal implementation.*/
int getpid(void){
	return 1;
}

/*Query whether output stream is a terminal*/
int _isatty(int file){
	return 1;
}

/*Send a signal*/
int kill(int pid, int sig){
	errno = EINVAL;
	return -1;
}

/*Establish a new name for an existing file*/
int link(char *old, char*new){
	errno = EMLINK;
	return -1;
}

int _lseek(int file, int ptr, int dir){
	return 0;
}

/*Open a file*/
int open(const char *name, int flags, int mode){
	return -1;
}

/* Read from a file*/
int _read(int file, char *ptr, int len){
	return 0;
}

/* Increase program data space. As malloc and related functions depend on this, it is useful to have a working implementation. The following suffices for a standalone system; it exploits the symbol _end automatically defined by the GNU linker */
caddr_t _sbrk( int incr ){
	extern char _end;
	static char* heap_end = 0;
	char* prev_heap_end;

	if( heap_end == 0 )
		heap_end = &_end;

	prev_heap_end = heap_end;

	heap_end += incr;
	return (caddr_t)prev_heap_end;
}

/*Status of a file(by name)*/
int stat(const char *file, struct stat *st){
	st->st_mode = S_IFCHR;
	return 0;
}

/*Timing information for current process*/
clock_t times(struct tms *buf){
	return -1;
}

/*Remove a file's directory entry*/
int unlink(char *name){
	errno = ENOENT;
	return -1;
}

/* Wait for for a child process*/
int wait(int *status){
	errno = ECHILD;
	return -1;
}

void outbyte(char b){
	RPI_AuxMiniUartWrite(b);
}

/*Write to a file. libc subroutines will use this system routine for output to all files, including stdout-so if you need to generate any output, for example to a serial port for debugging, you should make your minimal write capable of doing this. The following minimal implementation is an incomplete example; it relies on a outbyte subroutine*/

int _write(int file, char *ptr, int len){
	int todo;
	for(todo = 0; todo < len; todo++){
		outbyte(*ptr++);
	}
	return len;
}
