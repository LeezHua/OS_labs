#include <stdio.h>
#include <string.h>
#include "../system/parm.h"
#include "../system/user.h"
#include "../system/fs.h"
#include "../system/defs.h"

int
create(char* path, int type, int perm) {
  char name[FNAMESIZE] = {0};
  file pf, f;
  int ret;

  pf = nameparent(path, &f, name);

  if(pf) {
    if(f) {
      printf("File existed: %s\n", path);
      ret = 0;
    }
    else {
      f = falloc(pf, type);
      if(f) {
        f->readable = perm & PERM_R ? 'r': '-';
        f->writable = perm & PERM_W ? 'w': '-';
        strncpy(f->name, name, FNAMESIZE);
        ret = 1;
      }
      else {
        printf("Create failed: %s\n", path);
        ret = 0;
      }
    }
  }
  else {
    printf("Path not found: %s\n", path);
    ret = 0;
  }
  return ret;
}

int main(int argc, char* argv[]) {
  if(argc < 3) {
    printf("Usage: create <filename> <r/w/rw>\n");
    printf("       create -d <dirname>\n");
    return 0;
  }

  ureaddisk();
  binit();

  char* path;
  int type;
  int perm;

  if(strcmp(argv[1], "-d") == 0) {
    path = argv[2];
    type = T_DIR;
    perm = PERM_R | PERM_W;
  }
  else {
    path = argv[1];
    type = T_FILE;
    if(strcmp(argv[2], "r") == 0)
      perm = PERM_R;
    else if(strcmp(argv[2], "w") == 0)
      perm = PERM_W;
    else if(strcmp(argv[2], "rw") == 0)
      perm = PERM_R | PERM_W;
    else {
      printf("Usage: create <filename> <r/w/rw>\n");
      printf("       create -d <dirname>\n");
      return 0;
    }
  }
  if(create(path, type, perm)) {
    printf("Create succeed: %s\n", path);
  }

  bsave();

  return 0;
}