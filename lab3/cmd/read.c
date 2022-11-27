#include <stdio.h>
#include <string.h>
#include "../system/parm.h"
#include "../system/user.h"
#include "../system/fs.h"
#include "../system/defs.h"

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Usage: read <filename>\n");
    return 0;
  }

  ureaddisk();
  binit();

  file f;
  char* path = argv[1];
  char name[FNAMESIZE] = {0};

  nameparent(path, &f, name);

  if(!f) {
    printf("File not found: %s\n", path);
  }
  else {
    if(f->type == T_DIR) {
      printf("Not a file: %s\n", path);
    }
    else if(f->nlink == 0) {
      printf("Not opened: %s\n", path);
    }
    else if(f->readable != 'r') {
      printf("Not readable: %s\n", path);
    }
    else {
      char* src = (char*)baddr(f->addr);
      for(int i = 0; i < f->size; ++i)
        putchar(src[i]);
      putchar('\n');
    }
  }
  return 0;
}