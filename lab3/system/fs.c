#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parm.h"
#include "fs.h"
#include "user.h"

char disk[NBLOCKS][BSIZE];

char path[] = "./disk/block";

extern struct utable utable;

void itos(int num, char* str) {
  int p = 10;
  char ch;
  while(p) {
    ch = '0' + num/p;
    num %= p;
    p /= 10;
    strncat(str, &ch, 1);
  }
  str[14] = 0;
}

/* disk */

// create a disk with 1M storage capacity
void
breaddisk(int bno) {
  FILE* fp;
  char filename[32] = {0};
  strncat(filename, path, 13);
  // printf("%s\n", filename);
  itos(bno, filename);
  // printf("%s\n", filename);
  fp = fopen(filename, "r");
  fread(disk[bno], BSIZE, 1, fp);
  fclose(fp);
}

void
bwritedisk(int bno) {
  FILE* fp;
  char filename[32] = {0};
  strncat(filename, path, 13);
  itos(bno, filename);
  fp = fopen(filename, "w");
  fwrite(disk[bno], BSIZE, 1, fp);
  fclose(fp);
}

void
binit() {
  for(int i = 0; i < NBLOCKS; ++i)
    breaddisk(i);
}
void
bsave() {
  for(int i = 0; i < NBLOCKS; ++i)
    bwritedisk(i);
}

void*
baddr(int bno) {
  return (void*)disk[bno];
}

int
balloc() {
  char* b = (char*)baddr(MAPBLOCK);
  for(int i = DATASTART; i < BSIZE; ++i) {
    if(b[i] == 0) {
      memset(baddr(i), 0, BSIZE);
      b[i] = 1;
      return i;
    }
  }
  return 0;
}

/* file */

file
urootfile() {
  file f = (file)disk[1];
  for(int i = 0; i < FPB; ++i, ++f) {
    if(f->uid == utable.curuser)
      return f;
  }
  return NULL;
}

file
falloc(file pf, int type) {
  if(pf->type == T_FILE) {
    printf("falloc: is dir\n");
    exit(0);
  }

  if(pf->addr == 0)
    pf->addr = balloc();
  
  file f = ((file)baddr(pf->addr));
  for(int i = 0; i < FPB; ++i, ++f) {
    if(f->type == T_NONE)
      break;
  }
  memset(f, 0, sizeof(struct file));
  f->type = type;
  f->uid  = utable.curuser;
  f->addr = balloc();
  return f;
}

int
fdel(file f) {
  if(f->type == T_NONE) {
    printf("fdelete: error\n");
    return -1;
  }
  f->type = T_NONE;
  return 0;
}




/* path */

file
dirlookup(file f, char* name) {
  if(f->addr == 0)
    return NULL;
  file fp = (file)baddr(fp->addr);
  for(int i = 0; i < FPB; ++i, ++fp) {
    if(fp->type && strcmp(fp->name, name) == 0)
      return fp;
  }
  return NULL;
}

file
nameparent(char* path, file* f, char* name) {
  file pf = NULL;
  *f = urootfile();
  char* p = path;

  while(*p == '/')
    ++p;
  if(*p == 0)
    return pf;

  int n;
  
  while(*p) {
    n = 0;
    while(*p == '/')
      ++p;
    if(*p == 0)
      break;
    while(*p != '/' && *p)
      name[n++] = *p++;
    pf = *f;
    if(!pf)
      return NULL;
    if(pf->type == T_FILE)
      return pf;
    *f = dirlookup(pf, name);
  }
  return pf;
}

