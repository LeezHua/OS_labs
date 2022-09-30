#include "../head/proc.h"
#include <stdlib.h>

pcb build_pcb(){
  pcb p= (pcb)malloc(sizeof(struct pcb));
  if(!p) return NULL;

  return p;
}

pcb* build_nproc(int n){
  pcb* proc_array = (pcb*)malloc(n*sizeof(pcb));
  for(int i = 0; i < n; ++i){
    proc_array[i] = build_pcb();
  }
  return proc_array;
}

int set_proc(pcb p, int _pid, int _wait, int _server, int _grade){
  if(!p) return -1;

  p->pid    = _pid;
  p->wait   = _wait;
  p->server = _server;
  p->grade  = _grade;
  return 0;
}

int free_proc(pcb p){
  if(!p) return -1;
  
  free(p);
  return 0;
}