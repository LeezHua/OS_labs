#define FNAMESIZE 16
#define ROOTBLOCK 1
#define MAPBLOCK  2
#define DATASTART 3

#define T_NONE    0
#define T_FILE    1
#define T_DIR     2

#define PERM_R    (1 << 0)
#define PERM_W    (1 << 1)

typedef struct file {
  int uid;
  int type;
  char name[FNAMESIZE];
  int nlink;
  int size;
  char readable;
  char writable;
  int addr;
}*file;

#define FPB (BSIZE / sizeof(struct file))

typedef char block[BSIZE];
