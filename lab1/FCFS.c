#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "head/proc.h"
#include "head/cpu.h"
#include "head/procqueue.h"
#define MAX_BUF 128

void FCFS(proc_queue ready){
  int timeline = 0;
  int rf = 0;         //run to the ends flag
  int runtime;        //一个程序实际运行时间

  while(!empty_queue(ready)){
    printf("----------------------------------------------------------------------------------\n");
    printf("At timeline %d:\n", timeline);
    if(!rf){
      printf("Enter x to examine, or s to step one proc, or r to run to the ends, or e to exit\n");
      char buf[MAX_BUF] = {0};
      while(~scanf("%s", buf)){
        if(buf[0] == 'x' && buf[1] == 0)
          examine_queue(ready);
        else if(buf[0] == 's' && buf[1] == 0)
          break;
        else if(buf[0] == 'r' && buf[1] == 0){
          rf = 1;
          break;
        }
        else if(buf[0] == 'e' && buf[1] == 0)
          return;
        printf("Enter x to examine, or s to step one proc, or r to run to the ends, or e to exit\n");
      }
    }

    qnode_ptr node = ready->next;
    int nok = run_proc(node->p, &runtime);
    qnode_ptr tmp = node;
    delete_qnode(node);
    if(nok)
      pushback_qnode(ready, tmp);
    else
      free_qnode(tmp);
    
    timeline += runtime;
    update_queue(ready, runtime);
    if(!nok)
      examine_queue(ready);
    sleep(1);
  }

  printf("All jobs have finished\n");
}


int main(){
  int proc_nums = 6;
  pcb* procs = build_nproc(proc_nums);
  set_proc(procs[0], 3, 9, 5, 0);
  set_proc(procs[1], 5, 8, 8, 0);
  set_proc(procs[2], 1, 6, 2, 0);
  set_proc(procs[3], 7, 5, 6, 0);
  set_proc(procs[4], 9, 1, 9, 0);
  set_proc(procs[5], 4, 0, 1, 0);

  proc_queue ready = build_queue();
  for(int i = 0; i < proc_nums; ++i){
    qnode_ptr node = pack_proc(procs[i]);
    pushback_qnode(ready, node);
  }

  FCFS(ready);
  
  return 0;
}