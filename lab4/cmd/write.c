#include <stdio.h>
#include <string.h>
#include "../system/parm.h"
#include "../system/user.h"
#include "../system/fs.h"
#include "../system/defs.h"

int main(int argc, char* argv[]) {
  if(argc < 3) {
    printf("Usage: write <filename> <text>...\n");
    return 0;
  }

  ureaddisk();
  binit();

  file f;
  char name[FNAMESIZE] = {0};
  char* path = argv[1];
  char* text = argv[2];

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
    else if(f->writable != 'w') {
      printf("Not writable: %s\n", path);
    }
    else {
      char* dst = (char*)baddr(f->addr);
      strncpy(dst, text, strlen(text) + 1);
      f->size = strlen(text);
      printf("Write succeed\n");
    }
  }

  bsave();

  return 0;
}