#include "../head/procqueue.h"
#include <stdlib.h>
#include <stdio.h>


qnode_ptr build_queue(){
  qnode_ptr head = (qnode_ptr)malloc(sizeof(struct qnode));
  qnode_ptr rear = (qnode_ptr)malloc(sizeof(struct qnode));
  if(!head||!rear) return NULL;

  head->pre  = rear;
  head->next = rear;
  rear->pre  = head;
  rear->next = head;
  return head;
}

int empty_queue(proc_queue queue){
  if(queue->next->next == queue) return 1;
  return 0;
}

qnode_ptr pack_proc(pcb p){
  qnode_ptr node = (qnode_ptr)malloc(sizeof(struct qnode));
  if(!node) return NULL;

  node->p    = p;
  node->pre  = NULL;
  node->next = NULL;

  return node;
}

int pushback_qnode(proc_queue head, qnode_ptr node){
  if(!node || !head) return -1;

  qnode_ptr rear = head->pre;
  qnode_ptr last = rear->pre;

  last->next = node;
  node->pre  = last;
  node->next = rear;
  rear->pre  = node;

  return 0;
}

int delete_qnode(qnode_ptr node){
  if(!node) return -1;

  qnode_ptr pre  = node->pre;
  qnode_ptr next = node->next;
  pre->next = next;
  next->pre = pre;
  return 0;
}

int free_qnode(qnode_ptr node){
  if(!node) return -1;

  free_proc(node->p);
  free(node);
  return 0;
}

int examine_queue(proc_queue head){
  if(!head) return -1;
  int n = 0;
  qnode_ptr rear = head->pre;
  qnode_ptr iter = head->next;
  pcb p;

  printf("+--------------------------------------+\n");
  printf("|pid   |wait_time  |server_time|grade  |\n");
  printf("+--------------------------------------+\n");
  while(iter != rear){
    p = iter->p;
    printf("|%-6d|%-11d|%-11d|%-7.3f|\n", p->pid, p->wait, p->server,p->grade);
    printf("+--------------------------------------+\n");
    iter = iter->next;
    ++n;
  }
  printf("examine finished\n");
  return n;
}

int update_queue(proc_queue head, int seconds){
  if(!head) return -1;
  int n = 0;
  qnode_ptr rear = head->pre;
  qnode_ptr iter = head->next;
  pcb p;

  while(iter != rear){
    p = iter->p;
    p->wait += seconds;
    iter = iter->next;
    ++n;
  }
  return n;
}

qnode_ptr grade_proc(proc_queue head){
  if(!head) return NULL;
  qnode_ptr rear = head->pre;
  qnode_ptr iter = head->next;
  qnode_ptr res  = NULL;
  pcb p;


  while(iter != rear){
    p = iter->p;
    p->grade = (float)(p->wait + p->server) / p->server;
    if(!res)
      res = iter;
    else 
      if(p->grade > res->p->grade)
        res = iter;
    
    iter = iter->next;
  }
  return res;
}