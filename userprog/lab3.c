


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include <stdio.h>
#include <syscall-nr.h>
#include <list.h>

#include "devices/shutdown.h"
#include "devices/input.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "filesys/inode.h"
#include "filesys/directory.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/umem.h"
#include "userprog/lab3.h"



void sys_exit(int status) 
{
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}

void exit_handler(struct intr_frame *f) 
{
  int exitcode;
  umem_read(f->esp + 4, &exitcode, sizeof(exitcode));

  sys_exit(exitcode);
}

/*
 * BUFFER+0 and BUFFER+size should be valid user adresses
 */
uint32_t sys_write(int fd, const void *buffer, unsigned size)
{
  umem_check((const uint8_t*) buffer);
  umem_check((const uint8_t*) buffer + size - 1);

  int ret = -1;

  if (fd == 1) { // write to stdout
    putbuf(buffer, size);
    ret = size;
  }
  else{ // else, write to file
  }

  return (uint32_t) ret;
}

void write_handler(struct intr_frame *f)
{
    int fd;
    const void *buffer;
    unsigned size;

    umem_read(f->esp + 4, &fd, sizeof(fd));
    umem_read(f->esp + 8, &buffer, sizeof(buffer));
    umem_read(f->esp + 12, &size, sizeof(size));

    f->eax = sys_write(fd, buffer, size);
}

uint32_t sys_open(const char* file){
    if(!filesys_open(file)){
        return (uint32_t) -1;
    }
    return (uint32_t) filesys_open(file) * -1;
}

void open_handler(struct intr_frame *f){
    char* file;
    
    umem_read(f->esp + 4, &file, sizeof(file));
    f->eax = sys_open(file);
}

bool sys_create(const char* file, unsigned size){
    if(strlen(file) < 1 || strlen(file) > 16){
        return false;
    }
    lock_acquire(&syscallLock);
    bool success = filesys_create(file, size, false);
    lock_release(&syscallLock);
    return success;
}

void create_handler(struct intr_frame *f){
    char* path;
    unsigned size;
    
    umem_read(f->esp + 4, &path, sizeof(path));
    umem_read(f->esp + 8, &size, sizeof(size));
    
    f->eax = (uint32_t) sys_create(path, size);
}

bool sys_close(const char* file){
    printf("%s\n", file);
    lock_acquire(&syscallLock);
    bool success = filesys_close(file);
    lock_release(&syscallLock);
    return success;
}

void close_handler(struct intr_frame *f){
    char* path;
    
    umem_read(f->esp + 4, &path, sizeof(path));
    
    f->eax = (uint32_t) sys_close(path);
}

