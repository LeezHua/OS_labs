#include "proc.h"
#define TIMESLICE 2

//运行一个程序，未运行完成返回1
int run_proc(pcb, int*);
//运行中输出
void loading(int);