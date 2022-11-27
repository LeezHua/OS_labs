int sysopen(char* path, int omode);
int sysfstat(int fd, stat st);
int sysclose(int fd);
int sysread(int fd, char* buf, int n);
int syswrite(int fd, char* buf, int n);
int syscreate(char* path, int perm);
int sysmkdir(char* path);


#define O_READ   (1 << 0)
#define O_WRITE  (1 << 1)
#define O_APPEND (1 << 2)

#define PERM_R   (1 << 0)
#define PERM_W   (1 << 1)