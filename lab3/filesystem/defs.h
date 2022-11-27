#include "types.h"

// disk.c
void dread(void* buf, uint bno);
void dwrite(void* buf, uint bno);

//fs.c
uint  balloc();
inode ialloc(short type);
void  iupdate(inode ip);
inode idup(inode ip);
void  itrunc(inode ip);
void  iput(inode ip);
void  stati(inode ip, stat st);
int   readi(inode ip, void* dst, uint off, uint n);
int   writei(inode ip, void* src, uint off, uint n);
inode dirlookup(inode dp, char* name, uint* poff);
int   dirlink(inode dp, char* name, uint inum);
inode namei(char* path);
inode nameiparent(char* path, char* name);

//file.c
int filealloc(void);
file filedup(file f);
void fileclose(file f);
int filestat(file f, stat st);
int fileread(file f, void* addr, int n);
int filewrite(file f, void* addr, int n);