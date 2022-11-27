#include <stdio.h>
#include <string.h>
#include "parm.h"
#include "user.h"
#include "fs.h"

struct utable utable;

// create a new user
int
ucreate(char* uname, char* passwd) {
  user u;
  for(int i = 0; i < NUSERS; ++i) {
    u = &utable.users[i];
    if(!u->uid) {
      u->uid = i + 1;
      strncpy(u->name, uname, strlen(uname) + 1);
      strncpy(u->passwd, passwd, strlen(passwd) + 1);
      return u->uid;
    }
  }
  return 0;
}

// write utable to disk
int
uwritedisk() {
  block b = {0};
  memcpy(b, &utable, sizeof(utable));
  FILE* f = fopen("disk/block00", "w");
  fwrite(b, BSIZE, 1, f);
  fclose(f);
  return 0;
}

// read utable from disk
int
ureaddisk() {
  block b = {0};
  FILE* f = fopen("./disk/block00", "r");
  fread(b, BSIZE, 1, f);
  fclose(f);
  memcpy(&utable, b, sizeof(utable));
  return 0;
}

int
ucuruname(char* name) {
  strncpy(name, utable.users[utable.curuser-1].name, UNAMESIZE);
}

int
uverificat(char* uname, char* passwd) {
  user u;
  for(int i = 0; i < NUSERS; ++i) {
    u = &utable.users[i];
    if(strcmp(u->name, uname) == 0 && strcmp(u->passwd, passwd) == 0) {
      utable.curuser = u->uid;
      uwritedisk();
      return u->uid;
    }
  }
  return 0;
}