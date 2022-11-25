#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "filesystem/types.h"
#include "filesystem/stat.h"
#include "filesystem/fs.h"
#include "filesystem/file.h"
#include "filesystem/defs.h"
#include "filesystem/main.h"

#define BUFSIZE 256
#define MAXARGS 8

const char* cmdpath[] = {
[CMD_ls]     "user/ls",
[CMD_mkdir]  "user/mkdir",
[CMD_touch]  "user/touch",
[CMD_rm]     "user/rm"
};

int
gets(char* buf, int maxn) {
  int tot = 0;
  char ch;
  while((ch = getchar()) && ch != '\n' && tot + 1 < maxn) {
    buf[tot++] = ch;
  }
  buf[tot] = 0;
  return tot;
}

int
parseline(char* buf, char* argv[]) {
  int tot = 0;

  char* p = buf;
  while(*p == ' ')
    ++p;
  if(*p == 0)
    return CMD_none;

  while(*p != 0 && p < buf + BUFSIZE) {
    while(*p == ' ' && p < buf + BUFSIZE)
      *p++ = 0;
    if(*p) {
      argv[tot++] = p;
      while(*p != ' ' && *p && p < buf +BUFSIZE)
        ++p;
    }
  }
  argv[tot] = NULL;

  if(strcmp(argv[0], "ls") == 0)
    return CMD_ls;
  else if(strcmp(argv[0], "mkdir") == 0)
    return CMD_mkdir;
  else if(strcmp(argv[0], "touch") == 0)
    return CMD_touch;
  else if(strcmp(argv[0], "rm") == 0)
    return CMD_rm;
  else if(strcmp(argv[0], "exit") == 0)
    return CMD_exit;
  else
    return CMD_error;
}

int
runcmd(const char* path, char* argv[]) {
  if(fork() == 0)
    execv(path, argv);
  else
    wait(NULL);
}

int main() {
  char buf[BUFSIZE];
  int type;
  char* argv[MAXARGS];
  
  while(printf("$ ")) {
    gets(buf, BUFSIZE);
    int type = parseline(buf, argv);
    switch(type) {
      case CMD_none:
        break;
      case CMD_error:
        printf("command not found: %s\n", argv[0]);
        break;
      case CMD_ls:
        runcmd(cmdpath[CMD_ls], argv);
        break;
      case CMD_mkdir:
        runcmd(cmdpath[CMD_mkdir], argv);
        break;
      case CMD_touch:
        runcmd(cmdpath[CMD_touch], argv);
        break;
      case CMD_rm:
        runcmd(cmdpath[CMD_rm], argv);
        break;
      case CMD_exit:
        return 0;
      default:
        printf("error\n");
        return 0;
    }
  }
  

  return 0;
}