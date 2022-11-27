#include <stdio.h>
#include <string.h>
#include "../system/parm.h"
#include "../system/user.h"
#include "../system/fs.h"
#include "../system/defs.h"

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Usage: delete <filename>\n");
    return 0;
  }

  ureaddisk();
  binit();

  file f;
  char* path = argv[1];
  char name[FNAMESIZE] = {0};
  
  nameparent(path, &f, name);
  
  if(!f) {
    printf("File or dir not found: %s\n", path);
  }
  else {
    if(fdel(f) == 0) {
      printf("Delete succeed: %s\n", path);
    }
    else {
      printf("Delete failed: %s\n", path);
    }
  }
  bsave();

  return 0;
}