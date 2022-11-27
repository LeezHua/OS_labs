// user.c
int ucreate(char* uname, char* passwd);
int uwritedisk();
int ureaddisk();
int ucuruname(char* name);
int uverificat(char* uname, char* passwd);

// fs.c
void* baddr(int bno);
void  binit();
void  bsave();
file nameparent(char* path, file* f, char* name);

file falloc(file pf, int type);
int fdel(file f);