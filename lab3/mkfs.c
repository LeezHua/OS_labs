#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "system/parm.h"
#include "system/user.h"
#include "system/fs.h"
#include "system/defs.h"

char path[] = "./disk/block";
block b;

static void
itos(int num, char* str) {
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

// create a disk with 1M storage capacity
static void
createdisk() {
  FILE* fp;
  char filename[32] = {0};
  for(int i = 0; i < NBLOCKS; ++i) {
    strncat(filename, path, 13);
    // printf("%s\n", filename);
    itos(i, filename);
    // printf("%s\n", filename);
    fp = fopen(filename, "w");
    fwrite(b, BSIZE, 1, fp);
    fclose(fp);
    filename[0] = 0;
  }
}

// create 2 user
static void
createuser() {
  char* users[] = {
    "wang",
    "zhang"
  };
  char* passwds[] = {
    "wang123",
    "zhang123"
  };

  ucreate(users[0], passwds[0]);
  ucreate(users[1], passwds[1]);

  uwritedisk();
}

static void
createrootdir() {
  block b = {0};
  FILE* fp = fopen("./disk/block01", "w");

  // create wang's root dir
  file f = ((file)b);
  f->uid  = 1;
  f->type = T_DIR;
  strncpy(f->name, "/", 2);
  f->readable = 'r';
  f->writable = 'w';
  f->addr = 3;

  // create zhang's root dir
  ++f;
  f->uid   = 2;
  f->type  = T_DIR;
  strncpy(f->name, "/", 2);
  f->readable = 'r';
  f->writable = 'w';
  f->addr = 4;

  fwrite(b, BSIZE, 1, fp);
  fclose(fp);
  
  fp = fopen("./disk/block02", "w");
  memset(b, 0, BSIZE);
  b[0] = b[1] = b[2] = b[3] = b[4] = 1;
  fwrite(b, BSIZE, 1,fp);
  fclose(fp);
}

void
createfile() {
  FILE* fp = fopen("disk/block03", "w");
  struct file f = {0};
  f.readable = 'r';
  f.writable = 'w';
  strncpy(f.name, "test", 5);
  f.type = T_FILE;
  block b = {0};
  file ffp = (file)b;
  memcpy(ffp, &f, sizeof(f));
  fwrite(b, BSIZE, 1, fp);
  fclose(fp);
}

int main() {
  createdisk();
  createuser();
  createrootdir();
  // createfile();

  block b = {0};
  FILE* fp = fopen("disk/block01", "r");
  fread(b, BSIZE, 1, fp);
  file f = (file)b;
  for(int i = 0; i < FPB; ++i, ++f) {
    printf("%d, %s\n", f->uid, f->name);
  }
  fclose(fp);

  return 0;
}