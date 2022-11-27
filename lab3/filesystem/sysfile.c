#include <stdio.h>
#include "types.h"
#include "stat.h"
#include "fs.h"
#include "file.h"
#include "defs.h"
#include "sysfile.h"

extern struct file ftable[NFILE];

int
sysopen(char* path, int omode) {
  int fd;
  file f;
  inode ip;
  int n;

  if((ip = namei(path)) == 0)
    return -1;
  if(ip->type == T_DIR)
    return -1;
  
  // 分配文件描述符
  if((fd = filealloc()) < 0) 
    return -1;
  
  f = &ftable[fd];
  f->type = FD_INODE;
  f->off  = 0;
  f->ip = ip;
  f->readable = !(omode & O_WRITE & O_APPEND);
  f->writable = !(omode & O_READ);

  return fd;
}

int
sysfstat(int fd, stat st) {
  file f = &ftable[fd];
  return filestat(f, st);
}

int
sysclose(int fd) {
  file f = &ftable[fd];
  if(f->type == FD_NONE)
    return -1;
  fileclose(f);
  return 0;
}

int
sysread(int fd, char* buf, int n) {
  file f = &ftable[fd];
  if(f->type == FD_NONE)
    return 0;
  return fileread(f, buf, n);
}

int
syswrite(int fd, char* buf, int n) {
  file f = &ftable[fd];
  if(f->type == FD_NONE)
    return 0;
  return filewrite(f, buf, n);
}

int
syscreate(char* path, int perm) {
  inode ip, dp;
  char name[FILENAMESIZE];

  if((dp = nameiparent(path, name)) == 0)
    return 0;

  
}

int
sysmkdir(char* path) {

}

