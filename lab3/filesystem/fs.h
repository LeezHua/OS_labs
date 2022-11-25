// on-disk file system format

#define ROOTINO   1  // root i-number
#define BSIZE  1024  // block size (bytes)
#define FSSIZE 1024  // file system size (blocks)
#define NINODE   64  // max number of inode in memory
#define NDINODE 256  // max number of dinode on disk

#define NDIRECT  11
#define NINDIRECT (BSIZE / sizeof(uint))
#define MAXFILE   (NDIRECT + NINDIRECT)

#define INODESTART 32
#define BMAPSTART  47
#define DATASTART  48

// on-disk inode structure
typedef struct dinode {
  short type;         // file type
  short nlink;        // number of links to inode in file system
  uint size;          // size of file (bytes)
  uint addrs[NDIRECT + 1]; // Data block address
}*dinode;

// inodes per block
#define IPB   (BSIZE / sizeof(struct inode))

// block containing inode i
#define IBLOCK(i) (i / IPB + INODESTART)

// bitmap bits per block
#define BPB   (BSIZE * 8)

// block containing bit b
#define BBLOCK(b) (b / BPB + BMAPSTART)

#define FILENAMESIZE 14

typedef struct dirent {
  ushort inum;
  char name[FILENAMESIZE];
}*dirent;