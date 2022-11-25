/* support the interface of disk I/O */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

#define BSIZE 1024
#define BUFSIZE 32

static void
bno2block(uint bno, char* blockname) {
  strncpy(blockname, "./disk/block", 13);
  char ch;
  int p = 1000;
  while(p) {
    ch = '0' + bno/p;
    bno %= p;
    p /= 10;
    strncat(blockname, &ch, 1);
  }
  blockname[16] = 0;
}

// read a disk block
void
dread(void* buf, uint bno) {
  char blockname[BUFSIZE];
  
  bno2block(bno, blockname);
  FILE* fp = fopen(blockname, "r");
  fread(buf, 1, BSIZE, fp);
  fclose(fp);
}

// write a disk block
void
dwrite(void* buf, uint bno) {
  char blockname[BUFSIZE];
  bno2block(bno, blockname);
  FILE* fp = fopen(blockname, "w");
  fwrite(buf, 1, BSIZE, fp);
  fclose(fp);
}
