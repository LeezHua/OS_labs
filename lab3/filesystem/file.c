#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "stat.h"
#include "fs.h"
#include "file.h"
#include "defs.h"

struct file ftable[NFILE];

// allocate a file structure
file
filealloc(void) {
  file f;

  for(f = ftable; f < ftable + NFILE; ++f) {
    if(f->ref == 0) {
      f->ref = 1;
      return f;
    }
  }

  return 0;
}

// increment ref count for file f
file
filedup(file f) {
  if(f->ref < 1) {
    printf("filedup: error\n");
    exit(0);
  }
  f->ref++;
  return f;
}

// close file f.   (decrement ref count, close while reaches 0)
void
fileclose(file f) {
  struct file ff;
  if(f->ref < 1) {
    printf("fileclose: error\n");
    exit(0);
  }

  if(--f->ref > 0)
    return;
  
  ff = *f;
  f->ref = 0;
  f->type = FD_NONE;
  
  if(ff.type == FD_INODE)
    iput(ff.ip);
}

// get metadata about file f
int
filestat(file f, stat st) {
  if(f->type == FD_INODE) {
    stati(f->ip, st);
    return 0;
  }
  return -1;
}

// read from file f
int
fileread(file f, void* addr, int n) {
  int r = 0;
  if(f->readable == 0)
    return -1;
  
  if(f->type == FD_INODE) {
    if((r = readi(f->ip, addr, f->off, n)) > 0)
      f->off += r;
  }
  else {
    printf("fileread: error\n");
    exit(0);
  }

  return  r;
}

// write to file f
int
filewrite(file f, void* addr, int n) {
  int r, ret = 0;

  if(f->writable == 0)
    return -1;
  
  if(f->type == FD_INODE) {
    int i = 0;
    while(i < n) {
      int n1 = n - i;
      if((r = writei(f->ip, addr + i, f->off, n1)) > 0)
        f->off += r;
      
      if(r != n1)
        break;
      i += r;
    }
    ret = (i == n ? n : -1);
  }
  else {
    printf("filewrite: error\n");
    exit(0);
  }

  return ret;
}