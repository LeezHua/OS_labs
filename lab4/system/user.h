#define UNAMESIZE  16
#define PASSWDSIZE 16
typedef struct user {
  int uid;
  char name[UNAMESIZE];
  char passwd[PASSWDSIZE];
}*user;

struct utable {
  int curuser;
  struct user users[NUSERS];
};