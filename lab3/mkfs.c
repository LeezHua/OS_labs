#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem/types.h"
#include "filesystem/stat.h"
#include "filesystem/fs.h"
#include "filesystem/file.h"
#include "filesystem/defs.h"

#define DEBUG

char* path = "./disk/block";
char block[BSIZE];

void itos(int num, char* str) {
  int p = 1000;
  char ch;
  while(p) {
    ch = '0' + num/p;
    num %= p;
    p /= 10;
    strncat(str, &ch, 1);
  }
  str[16] = 0;
}

// create a disk with 1M storage capacity
void createdisk() {
  
  FILE* fp;
  char filename[32] = {0};
  for(int i = 0; i < FSSIZE; ++i) {
    strncat(filename, path, 13);
    // printf("%s\n", filename);
    itos(i, filename);
    // printf("%s\n", filename);
    fp = fopen(filename, "w");
    fwrite(block, BSIZE, 1, fp);
    fclose(fp);
    filename[0] = 0;
  }
}

// initialize meta block
// set bit 0 to 47 in bitmap block
// represents those blocks are used
void initmeta() {
  char* bp = (char*)malloc(BSIZE);

  dread(bp, BMAPSTART);
  
  for(int i = 0; i < 6; ++i)
    bp[i] = 0xff;
  
  dwrite(bp, BMAPSTART);
  free(bp);
}

// create root directory
void createroot() {
  struct dinode root, *r;
  struct dirent ent, *e;
  uint bno;
  char* bp = (char*)malloc(BSIZE);

  memset(&root, 0, sizeof(root));

  root.type = T_DIR;
  root.nlink = 1;     // for '..'
  root.size  = 32;    // for '.' and '..' dirents
  root.addrs[0] = bno = balloc();

  // append root dinode
  memset(bp, 0, BSIZE);
  r = (dinode)bp + 1;
  memcpy(r, &root, sizeof(root));
  dwrite(bp, INODESTART);

  // create dirents
  memset(bp, 0, BSIZE);
  e = (dirent)bp;

  memset(&ent, 0, sizeof(ent));
  ent.inum = 1;
  memcpy(ent.name, ".", 1);
  memcpy(&e[0], &ent, sizeof(ent));

  memset(&ent, 0, sizeof(ent));
  ent.inum = 1;
  memcpy(ent.name, "..", 2);
  memcpy(&e[1], &ent, sizeof(ent));
  
  dwrite(bp, bno);

  free(bp);
}

int main() {
  
  createdisk();
  initmeta();
  createroot();

  #ifdef DEBUG

  unsigned char* bp = (char*)malloc(BSIZE);
  dirent e;
  dinode i;
  uint bno, size;

  dread(bp, INODESTART);
  i = (dinode)bp;
  printf("%d, %d, %d\n", i[1].type, i[1].nlink, size = i[1].size);
  bno = i[1].addrs[0];
  dread(bp, bno);
  for(int j = 0; j < size; j += sizeof(struct dirent)) {
    e = (dirent)(bp + j);
    printf("%d, %s\n", e->inum, e->name);
  }

  free(bp);

  #endif

  return 0;
}