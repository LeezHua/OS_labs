# disk layout
disk size: 1024 byte
- 00-00: boot
- 01-01: super
- 02-31: meta
- 32-46: inode
- 47-47: bit map
- 48: 1023: data

# dinode (on-disk index of file)
dinode size: 32 byte
- type    file type
- nlink   number of links
- size    size of file (bytes)
- addrs[12] data block addresses  
  addr[0, 10] direct block number
  addr[11] indirect block number

# inode (in-memory index of file)
inode size 32 byte
- inum   dinode number
- 
- type   file type
- nlink  number of links
- size   size of file (bytes)
- addrs  data block addresses

# dirent (directory entry)
entry size 16 byte
- inum: 2 byte
- fname: 14 byte

# root directory
inum = 1  
block number = 32  
initial entries  
  - 1, '.'  
  - 1, '..'

initial size = 32 byte

# file system API
## syslist
  