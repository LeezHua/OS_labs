#define T_FILE 1
#define T_DIR  2

typedef struct stat {
  uint ino;
  short type;
  short nlink;
  uint size;
  char readable;
  char writable;
}*stat;