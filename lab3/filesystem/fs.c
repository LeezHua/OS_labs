/* This file contains the low-level file system manipulation routines */

#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include "types.h"
#include "stat.h"
#include "fs.h"
#include "file.h"
#include "defs.h"

#define min(a, b) (a > b ? a : b)

// zero a block
static void
bzeroo(int bno) {
  char* bp = (char*)malloc(BSIZE);
  
  dread(bp , bno);
  memset(bp, 0, BSIZE);
  dwrite(bp, bno);

  free(bp);
}

// allocate a zeroed disk block
uint
balloc() {
  int bi, m;
  char* bp = (char*)malloc(BSIZE);

  dread(bp, BMAPSTART);
  for(bi = DATASTART; bi < BPB && bi < FSSIZE; ++bi) {
    m = 1 << (bi % 8);
    if((bp[bi/8] & m) == 0) { // free block
      bp[bi/8] |= m;
      dwrite(bp, BMAPSTART);
      bzeroo(bi);
      free(bp);
      return bi;
    }
  }

  free(bp);
  printf("balloc: out of blocks\n");
  exit(0);
}

// free a disk block
static void
bfree(uint b) {
  char* bp = (char*)malloc(BSIZE);
  int bi, m;

  dread(bp, BBLOCK(b));
  bi = b % BPB;
  m = 1 << (bi % 8);
  if((bp[bi/8] & m) == 0) {
    free(bp);
    printf("freeing free block\n");
    exit(0);
  }

  bp[bi/8] &= ~m;
  dwrite(bp, b);
  
  free(bp);
}

/* inodes */

struct inode itable[NINODE];

// find the inode with number inum on disk
// and return the in-memory copy

static inode
iget(uint inum) {
  inode ip, empty;

  // is the inode already in the talbe?

  empty = 0;
  for (ip = itable; ip < &itable[NINODE]; ++ip) {
    if(ip->ref > 0 && ip->inum == inum) {
      ++ip->ref;
      return ip;
    }
    if(empty == 0 && ip->ref == 0)
      empty = ip;
  }

  if(empty == 0) {
    printf("iget: no inodes\n");
    exit(0);
  }

  ip = empty;
  ip->inum = inum;
  ip->ref  = 1;

  return ip;
}

// allocate an inode
inode
ialloc(short type) {
  int inum;
  char* bp = (char*)malloc(BSIZE);
  dinode dip;

  for(inum = 1; inum < NDINODE; ++inum) {
    dread(bp, IBLOCK(inum));
    dip = (dinode)bp + inum % IPB;
    if(dip->type == 0) { // a free inode
      memset(dip, 0, sizeof(struct dinode));
      dip->type = type;
      dwrite(bp, IBLOCK(inum));
      
      free(bp);
      return iget(inum);
    }
  }
  free(bp);
  printf("ialloc: no inodes\n");
  exit(0);
}

// copy a modified in-memory inode to disk
void
iupdate(inode ip) {
  char* bp = (char*)malloc(BSIZE);
  dinode dip;

  dread(bp, IBLOCK(ip->inum));
  dip = (dinode)bp + ip->inum % IPB;
  dip->type = ip->type;
  dip->nlink = ip->nlink;
  dip->size = ip->size;
  memcpy(dip->addrs, ip->addrs, sizeof(ip->addrs));
  dwrite(bp, IBLOCK(ip->inum));

  free(bp);
}

// increment reference count for ip
inode
idup(inode ip) {
  ++ip->ref;
  return ip;
}

// truncate inode
void
itrunc(inode ip) {
  int i, j;
  char* bp = (char*)malloc(BSIZE);
  uint* a;

  for(i = 0; i < NDIRECT; ++i) {
    if(ip->addrs[i]) {
      bfree(ip->addrs[i]);
      ip->addrs[i] = 0;
    }
  }

  if(ip->addrs[NDIRECT]) {
    dread(bp, ip->addrs[NDIRECT]);
    a = (uint*)bp;
    for(j = 0; j < NINDIRECT; ++j) {
      if(a[j])
        bfree(a[j]);
    }
    bfree(ip->addrs[NINDIRECT]);
    ip->addrs[NINDIRECT] = 0;
  }

  ip->size = 0;
  iupdate(ip);
  free(bp);
}

// drop a reference to an in-memory inode
void
iput(inode ip) {
  // last reference and has no links
  if(ip->ref == 1 && ip->nlink == 0) {
    itrunc(ip);
    ip->type = 0;
    iupdate(ip);
  }
  --ip->ref;
}

// return the disk block address of the nth block in inode ip
// if there is no such block, bmap allocates one
static uint
bmap(inode ip, uint bnth) {
  uint addr, *a;
  char* bp = (char*)malloc(BSIZE);

  if(bnth < NDIRECT) {
    if((addr = ip->addrs[bnth]) == 0)
      ip->addrs[bnth] = addr = balloc();
    free(bp);
    return addr;
  }
  bnth -= NDIRECT;

  if(bnth < NINDIRECT) {
    if(bnth < NINDIRECT) {
      if((addr = ip->addrs[NDIRECT]) == 0)
        ip->addrs[NDIRECT] = addr = balloc();
      dread(bp, addr);
      a = (uint*)bp;
      if((addr = a[bnth]) == 0) {
        a[bnth] = addr = balloc();
        dwrite(bp, ip->addrs[NDIRECT]);
      }
    }
    free(bp);
    return addr;
  }

  free(bp);
  printf("bmap: out of range\n");
  exit(0);
}

// copy filestat information from inode
void
stati(inode ip, stat st) {
  st->ino = ip->inum;
  st->type = ip->type;
  st->nlink = ip->nlink;
  st->size = ip->size;
}

// read data from inode
int
readi(inode ip, void* dst, uint off, uint n) {
  uint tot, m;
  char* bp = (char*)malloc(sizeof(BSIZE));
  if(off > ip->size || off + n < off)
    return 0;
  if(off + n > ip->size)
    n = ip->size - off;
  
  for(tot = 0; tot < n; tot += m, off += m, dst += m) {
    dread(bp, bmap(ip, off/BSIZE));
    m = min(n - tot, BSIZE - off%BSIZE);
    memcpy(dst, bp + (off + BSIZE), m);
  }

  free(bp);
  return tot;
}

// write data to inode
int
writei(inode ip, void* src, uint off, uint n) {
  uint tot, m;
  char* bp = malloc(BSIZE);

  if(off > ip->size || off + n < off)
    return -1;
  if(off + n > MAXFILE*BSIZE)
    return -1;
  
  for(tot = 0; tot < n; tot += m, off += m, src += m) {
    dread(bp, bmap(ip, off/BSIZE));
    m = min(n - tot, BSIZE - off % BSIZE);
    memcpy(bp + (off % BSIZE), src, m);
    dwrite(bp, ip->addrs[off/BSIZE]);
  }
  free(bp);
  
  if(off > ip->size)
    ip->size = off;
  iupdate(ip);

  return tot;
}

/* directories */

// look for a directory entry in a directroy
// if found, set *poff to byte offset of entry
inode
dirlookup(inode dp, char* name, uint* poff) {
  uint off, inum;
  struct dirent de;

  if(dp->type != T_DIR) {
    printf("dirlookup: not a DIR\n");
    exit(0);
  }

  for(off = 0; off < dp->size; off += sizeof(de)) {
    if(readi(dp, &de, off, sizeof(de)) != sizeof(de)) {
      printf("dirlookup: read\n");
      exit(0);
    }
    if(de.inum == 0)
      continue;
    if(strncmp(name, de.name, FILENAMESIZE) == 0) {
      if(poff)
        *poff = off;
      inum = de.inum;
      return iget(inum);
    }
  }

  return 0;
}

// write a new directory entry into the directory dp
int
dirlink(inode dp, char* name, uint inum) {
  int off;
  struct dirent de;
  inode ip;

  if((ip = dirlookup(dp, name, 0)) != 0) {
    iput(ip);
    return -1;
  }

  // look for an empty dirent
  for(off = 0; off < dp->size; off += sizeof(de)) {
    if(readi(dp, &de, off, sizeof(de)) != sizeof(de)) {
      printf("dirlink: read\n");
      exit(0);
    }
    if(de.inum == 0)
      break;
  }

  strncpy(de.name, name, FILENAMESIZE);
  de.inum = inum;
  if(writei(dp, &de, off, sizeof(de)) != sizeof(de)) {
    printf("dirlink: write\n");
    exit(0);
  }

  return 0;
}


static char*
skipelem(char* path, char* name) {
  char* s;
  int len;

  while(*path == '/')
    ++path;
  if(*path == 0)
    return 0;
  s = path;
  while(*path != '/' && *path != 0)
    ++path;
  len = path - s;
  if(len >= FILENAMESIZE)
    memcpy(name, s, FILENAMESIZE);
  else {
    memcpy(name, s, len);
    name[len] = 0;
  }

  while(*path == '/')
    ++path;
  return path;
}

static inode
namex(char* path, int nameiparent, char* name) {
  inode ip, next;

  if(*path == '/')
    ip = iget(ROOTINO);
  else {
    printf("namex: not from root\n");
    exit(0);
  }

  while((path = skipelem(path, name)) != 0) {
    if(ip->type != T_DIR)
      return 0;
    if(nameiparent && *path == '\0')
      return ip;
    if((next = dirlookup(ip, name, 0)) == 0)
      return 0;
    ip = next;
    if(nameiparent) {
      iput(ip);
      return 0;
    }
  }
  return ip;
}

inode
namei(char* path) {
  char name[FILENAMESIZE];
  return namex(path, 0, name);
}

inode
nameiparent(char* path, char* name) {
  return namex(path, 1, name);
}