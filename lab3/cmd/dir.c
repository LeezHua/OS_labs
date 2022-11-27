#include <stdio.h>
#include "../system/parm.h"
#include "../system/user.h"
#include "../system/fs.h"
#include "../system/defs.h"

extern int curuser;

void
fmtprint(file f) {
  char t, r, w;
  int size, addr;

  if(f->type == T_DIR)
    t = 'd';
  else
    t = '-';
  
  r = f->readable;
  w = f->writable;
  size = f->size;
  addr = f->addr * BSIZE;

  printf("%c %c%c  %-8d0x%04x    %s\n", t, r, w, size, addr, f->name);
}

void
dir(char* path) {
  file pf, f, fc;
  char name[FNAMESIZE] = {0};
  pf = nameparent(path, &f, name);

  if(!f || (pf == f)) {
    printf("Path not found: %s\n", path);
    return;
  }
  
  printf("%s: \n", path);
  if(f->type == T_FILE){
    fmtprint(f);
  }
  else if(f->type == T_DIR) {
    if(f->addr)
      fc = (file)baddr(f->addr);
    else {
      return;
    }
    for(int i = 0; i < FPB; ++i, ++fc) {
      if(fc->type != T_NONE)
        fmtprint(fc);
    }
  }
  else
    printf("nothing\n");
}

int main(int argc, char* argv[]) {
  ureaddisk();
  binit();

  if(argc < 2) {
    dir("/");
    return 0;
  }

  for(int i = 1; i < argc; ++i) {
    dir(argv[i]);
  }

  return 0;
}