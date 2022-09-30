#ifndef __PROCQUEUEH__
#define __PROCQUEUEH__

#include "proc.h"

//带首尾双向链表
typedef struct qnode{
  pcb p;
  struct qnode* pre;
  struct qnode* next;
}*qnode_ptr;
typedef qnode_ptr proc_queue;

//新建一个进程队列
qnode_ptr build_queue();
//判断队列是否为空
int empty_queue(proc_queue queue);
//封装一个进程为结点
qnode_ptr pack_proc(pcb);
//向进程队列中插入一个进程结点
int pushback_qnode(proc_queue, qnode_ptr);
//向进程队列中删除一个进程结点
int delete_qnode(qnode_ptr);
//释放一个进程结点
int free_qnode(qnode_ptr);
//检查队列状态
int examine_queue(proc_queue);
//更新队列状态
int update_queue(proc_queue, int);
//计算每个进程的优先级并返回优先级最高的结点
qnode_ptr grade_proc(proc_queue);

#endif