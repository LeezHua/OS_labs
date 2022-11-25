// in-memory copy of an inode
typedef struct inode {
  uint  inum;
  int   ref; 
  short type;
  short nlink;
  uint  size;
  uint  addrs[NDIRECT + 1];
}*inode;

// file control block
typedef struct file {
  enum {FD_NONE, FD_INODE} type;
  char readable;
  char writable;
  int ref;
  inode ip;
  uint off;
}*file;

#define NFILE 128

