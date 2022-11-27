#include <stdio.h>
#include "../system/parm.h"
#include "../system/user.h"
#include "../system/fs.h"
#include "../system/defs.h"

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Usage: open <filename>\n");
    return 0;
  }

  ureaddisk();
  binit();

  file f;
  char name[FNAMESIZE] = {0};
  char* path = argv[1];
  
  nameparent(path, &f, name);

  if(!f) {
    printf("Close failed: %s\n", path);
  }
  else {
    if(f->nlink == 0) {
      printf("Not opened: %s\n", path);
    }
    else {
      --f->nlink;
      printf("Close succeed: %s\n", path);
    }
  }

  bsave();

  return 0;
}