#include "vm.h"
#include <time.h>
#include <stdio.h>
#define INSTRS 320                    // 指令条数

pagetable_t pagetable;                // 页表
byte pmem[VMPAGES * PGSIZE];          // 物理内存
int instrseq[INSTRS + 1];             // 指令序列
uint pc;                              // PC寄存器
int pages;                            // 物理内存页数
double resfifo[33], reslru[33];       // 两种算法打命中率

// 随机生成[m, n]的随机数
int exrand(int m, int n){
  if(m <= n){
    int s = n - m + 1;
    return rand() % s + m;
  }
  return RAND_MAX;
}

// 随机生成指令序列
void make_instrs(){
  int n = 0;
  srand(time(0));
  instrseq[n++] = rand() % INSTRS;
  for(;n < INSTRS+1; n += 4){
    instrseq[n]   = (instrseq[n-1] + 1) % INSTRS;
    instrseq[n+1] = exrand(0, instrseq[n]);
    instrseq[n+2] = (instrseq[n+1] + 1) % INSTRS;
    instrseq[n+3] = exrand((instrseq[n+2] + 2) % INSTRS, INSTRS);
  }
}

// 一些初始化工作
void init(){
  memset(pagetable, 0, sizeof(pagetable));
  pc = INSTR2VA(instrseq[0]);
  pte_t pte = pagetable[PX(pc)];
  pte = PA2PTE(0) | PTE_E;
}

// 先进先出替换算法
void FIFO(){
  init();
}

// 最近最久未使用替换算法
void LRU(){
  
}

int main(){
  pagetable = make_pagetable();
  make_instrs();

  for(pages = 4; pages <= 32; ++pages){
    
  }


  free_pagetable(pagetable);


  return 0;
}