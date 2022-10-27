#include "../head/cpu.h"
#include <stdio.h>
#include <unistd.h>

int run_proc(pcb p, int* runtime){
  if(p->server <= 0) {
    printf("ERROR\n");
    return 0;
  }

  printf("process %d: running\n", p->pid);
  if(p->server <= TIMESLICE){
    *runtime  = p->server;
    loading(p->server);
    p->server = 0;
    p->wait   = 0;
    printf("process %d: complete\n", p->pid);
    return 0;
  }
  else{
    *runtime   = TIMESLICE;
    loading(TIMESLICE);
    p->server -= TIMESLICE;
    p->wait    = -2;
    printf("process %d: timeslice use up\n", p->pid);
    return 1;
  }
}

int run_job(pcb p, int* runtime){
  if(p->server <= 0){
    printf("error\n");
    return 0;
  }

  printf("job %d: running\n", p->pid);
  loading(p->server);
  *runtime  = p->server;
  p->server = 0;
  p->wait   = 0;
  printf("job %d: complete\n", p->pid);
  return 0;
}

void loading(int seconds){
  while(seconds--){
    printf(".\n");
    sleep(1);
  }
}