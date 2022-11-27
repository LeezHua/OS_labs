#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "system/parm.h"
#include "system/user.h"
#include "system/fs.h"
#include "system/defs.h"
#include "system/cmd.h"

#define BUFSIZE 256
#define MAXARGS 8

const char* cmdpath[] = {
[CMD_close]   "./cmd/close",
[CMD_create]  "./cmd/create",
[CMD_delete]  "./cmd/delete",
[CMD_dir]     "./cmd/dir",
[CMD_login]   "./cmd/login",
[CMD_logout]  "./cmd/logout",
[CMD_open]    "./cmd/open",
[CMD_read]    "./cmd/read",
[CMD_write]   "./cmd/write"
};

int curuser;

int
runcmd(const char* path, char* argv[]) {
  if(fork() == 0){
    execv(path, argv);
  }
  else
    wait(NULL);
}

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
  int quote = 0;

  char* p = buf;
  while(*p == ' ')
    ++p;
  if(*p == 0)
    return CMD_none;

  

  while(*p != 0 && p < buf + BUFSIZE) {
    while(*p == ' ' && p < buf + BUFSIZE)
      *p++ = 0;
    if(*p) {
      if(*p == '"') {
        *p++ = 0;
        quote = 1;
      }
      argv[tot++] = p;
      if(!quote)
        while(*p != ' ' && *p && p < buf + BUFSIZE)
          ++p;
      else {
        while(*p && *p != '"' && p < buf + BUFSIZE)
          ++p;
        *p = ' ';
        quote = 0;
      }
    }
  }
  argv[tot] = NULL;

  if(strcmp(argv[0], "close") == 0)
    return CMD_close;
  else if(strcmp(argv[0], "create") == 0)
    return CMD_create;
  else if(strcmp(argv[0], "delete") == 0)
    return CMD_delete;
  else if(strcmp(argv[0], "dir") == 0)
    return CMD_dir;
  else if(strcmp(argv[0], "login") == 0)
    return CMD_login;
  else if(strcmp(argv[0], "logout") == 0)
    return CMD_logout;
  else if(strcmp(argv[0], "open") == 0)
    return CMD_open;
  else if(strcmp(argv[0], "read") == 0)
    return CMD_read;
  else if(strcmp(argv[0], "write") == 0)
    return CMD_write;
  else
    return CMD_error;
}

int
login() {
  char uname[UNAMESIZE];
  char passwd[PASSWDSIZE];
  memset(uname, 0, sizeof(uname));
  memset(passwd, 0, sizeof(passwd));

  FILE* fp = fopen("source/login.txt", "r");
  char ch;
  while((ch = fgetc(fp)) != EOF) {
    putchar(ch);
  }
  fclose(fp);

  while(!curuser) {
    printf("user: ");
    scanf("%s", uname);
    printf("password: ");
    scanf("%s", passwd);
    uname[strlen(uname)] = 0;
    passwd[strlen(passwd)] = 0;

    curuser = uverificat(uname, passwd);
    getchar();
  }
  return 1;
}

void
init() {
  ureaddisk();
}

int main() {
  char buf[BUFSIZE];
  int type;
  char* argv[MAXARGS];
  char uname[UNAMESIZE];

  init();
  while(login()) {
    FILE* fp = fopen("source/wellcom.txt", "r");
    char ch;
    while((ch = fgetc(fp)) != EOF) {
      putchar(ch);
    }
    fclose(fp);
    ucuruname(uname);
    while(1) {
      printf("%s$ ", uname);

      gets(buf, BUFSIZE);
      type = parseline(buf, argv);
      switch(type) {
        case CMD_none:
          break;
        case CMD_close:
          runcmd(cmdpath[CMD_close], argv);
          break;
        case CMD_create:
          runcmd(cmdpath[CMD_create], argv);
          break;
        case CMD_delete:
          runcmd(cmdpath[CMD_delete], argv);
          break;
        case CMD_dir:
          runcmd(cmdpath[CMD_dir], argv);
          break;
        case CMD_error:
          printf("command not found: %s\n", argv[0]);
          break;
        case CMD_logout:
          curuser = 0;
          break;
        case CMD_open:
          runcmd(cmdpath[CMD_open], argv);
          break;
        case CMD_read:
          runcmd(cmdpath[CMD_read], argv);
          break;
        case CMD_write:
          runcmd(cmdpath[CMD_write], argv);
          break;
        default:
          return 0;
      }
      if(curuser == 0)
        break;
    }
  }

  return 0;
}