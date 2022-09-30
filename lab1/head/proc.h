#ifndef __PROCH__
#define __PROCH__

#define MAX_PCB 16
typedef struct pcb{
  int pid;      
  int wait;     //等待时间
  int server;   //要求服务时间
  float grade;  //优先级
}*pcb;

//建立一个进程
pcb build_proc();
//建立n个进程，返回一个pcb数组
pcb* build_nproc(int);
//设置进程信息
int set_proc(pcb, int, int, int, int);
//释放一个进程
int free_proc(pcb);

#endif