#include "stdlib.h"
#include "stdio.h"
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/mman.h>

#define BUFSIZE 8
#define SYSCALL_ERR -1

typedef enum {
   READ = O_RDONLY,
   WRITE = O_WRONLY,
   APPEND = O_APPEND,
   READWRITE = O_RDWR
} FileMode;


typedef struct {
   int fd;
   FileMode file_mode;
} File;



File* my_fopen(const char* filename, FileMode mode){
   int fd = syscall(SYS_open,filename,mode);
   File* file = (File*) malloc(sizeof(File));
   assert(file);

   file->fd = fd;
   file->file_mode = mode;
   return file;
}

bool my_fread(File* fp,void* buffer,size_t size){
   FileMode mode = fp->file_mode;
   if (mode != READ && mode != READWRITE)
      return false;
   
   int result =  syscall(SYS_read,fp->fd,buffer,size);
   if (result == SYSCALL_ERR)
      return false;
   return true;
}  


int main(int argc, char *argv[]){
   
   // File* fp = my_fopen("texto.txt",READ);
   // char buf[BUFSIZE+1];
   // buf[BUFSIZE] = '\0';
   // my_fread(fp,buf,BUFSIZE);
   // printf("text: %s \n",buf);


   File* fp = my_fopen("texto.txt",READ);
   char* pointer = (char*) syscall(SYS_mmap,NULL,3,PROT_READ,MAP_SHARED,fp->fd,0);
   printf("result: %p \n",pointer);

   printf("char 1: %c \n", (char) pointer[10]);


}