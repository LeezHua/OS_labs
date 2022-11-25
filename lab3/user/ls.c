#include <stdio.h>
#include "../filesystem/types.h"
#include "../filesystem/stat.h"
#include "../filesystem/fs.h"
#include "../filesystem/sysfile.h"

void
printfile(stat st) {

}

// void
// ls(char* path) {
//   char buf[512], *p;
//   int fd;
//   dirent de;
//   struct stat st;

//   if((fd = sysopen(path, 0)) < 0) {
//     printf("ls: cannot open %s\n", path);
//     return;
//   }

//   if(sysfstat(fd, &st) < 0) {
//     printf("ls: cannot stat %s\n", path);
//     sysclose(fd);
//     return;
//   }

//   if(st.type == T_FILE) {
//     printfile(&st);
//   } else if(st.type == T_DIR) {
//     strcpy(buf, path);
//   }
// }

int main(int argc, char* argv[]) {
  // if(argc < 2) {
  //   ls(".");
  //   exit(0);
  // }

  // for(int i = 1; i < argc; ++i)
  //   ls(argv[i]);
  
  // exit(0);
  printf("ls: running\n");
  return 0;
}