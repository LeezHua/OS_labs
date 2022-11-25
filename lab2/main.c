#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#define INSTRS 320                    // 指令条数
#define inf 0x3f3f3f3f

pagetable_t pagetable;                // 页表
int instrseq[INSTRS + 1];             // 指令序列
uint pc;                              // PC寄存器
int pages;                            // 物理内存页数
double fifoPHR[33], lruPHR[33];       // 两种算法打命中率

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
  for(; n < INSTRS+1; n += 4){
    instrseq[n]   = (instrseq[n-1] + 1) % INSTRS;
    instrseq[n+1] = exrand(0, instrseq[n]);
    instrseq[n+2] = (instrseq[n+1] + 1) % INSTRS;
    instrseq[n+3] = exrand((instrseq[n+2] + 2) % INSTRS, INSTRS);
  }
}

// 一些初始化工作
void init(){
  memset(pagetable, 0, VMPAGES * sizeof(pte_t));
  pc = INSTR2VA(instrseq[0]);
  pte_t* pte = &pagetable[PX(pc)];
  int pa = alloc_page();
  if(pa < 0){
    printf("init error\n");
    exit(0);
  }
  *pte = PA2PTE(pa) | PTE_V;
}

// 先进先出替换算法
void FIFO(){
  init();

  int n = 0;        // 缺页次数
  pte_t* replace[VMPAGES];
  uint next = 0;
  replace[next++] = &pagetable[PX(pc)];
  
  for(int i = 1; i < INSTRS + 1; ++i){
    pc = INSTR2VA(instrseq[i]);
    pte_t* pte = &pagetable[PX(pc)];
    if(*pte & PTE_V){     // 页面存在

    }
    else{                 // 页面不存在
      ++n;

      int pa;
      if((pa = alloc_page()) < 0){   // 物理内存不足，页面替换
        pte_t* tpte = replace[next]; // 取得替换页面的pte
        pa          = PTE2PA(*tpte); // 物理页面的地址
        *tpte      &= ~PTE_V;        // 解除原来的映射
      }
      *pte = PA2PTE(pa) | PTE_V;     // 映射缺失页面
      replace[next] = pte;           // 更新替换页面队列
      next = (next + 1) % pages;
    }
  }

  fifoPHR[pages] = (double)(INSTRS - n) / INSTRS;
}

// 最近最久未使用替换算法
void LRU(){
  init();

  int n = 0;    // 缺页次数
  uint pagetime[VMPAGES], timeline = 0;
  memset(pagetime, 0, sizeof(pagetime));
  pagetime[PX(pc)] = ++timeline;
  

  for(int i = 1; i < INSTRS; ++i){
    pc = INSTR2VA(instrseq[i]);
    pte_t* pte = &pagetable[PX(pc)];
    // 命中
    if(*pte & PTE_V){

    }
    // 缺页
    else{
      ++n;
      int pa;
      if((pa = alloc_page()) < 0){  // 内存不足，页面替换
        pte_t* tpte;
        uint mintime = inf;
        for(uint va = 0; va < VMPAGES * PGSIZE; va += PGSIZE){
          if((pagetable[PX(va)] & PTE_V) == 0)
            continue;
          if(pagetime[PX(va)] < mintime){
            mintime = pagetime[PX(va)];
            tpte    = &pagetable[PX(va)];
          }
        }
        pa     = PTE2PA(*tpte);
        *tpte &= ~PTE_V;
      }
      *pte = PA2PTE(pa) | PTE_V;
    }
    pagetime[PX(pc)] = ++timeline;
  }

  lruPHR[pages] = (double)(INSTRS - n) / INSTRS;
}

int main(){
  pagetable = make_pagetable();
  make_instrs();

  for(pages = 4; pages <=32; ++pages){
    init_mempool(pages);
    FIFO();
    free_mempool();

    init_mempool(pages);
    LRU();
    free_mempool();
  }
  printf("+-----+----------+----------+\n"
         "|\\替换| 最近最久 | 先进先出 |\n"
         "| \\ 算|  未使用  |          |\n"
         "|内\\法|          |          |\n"
         "|存 \\ | 命中率/%% | 命中率/%% |\n"
         "|页数\\|          |          |\n"
         "+-----+----------+----------+\n");
  for(int i = 4; i <= 32; ++i){
    printf("| %-4d|  %-8.4lf|  %-8.4lf|\n", i, lruPHR[i], fifoPHR[i]);
    printf("+-----+----------+----------+\n");
  }
  

  free_pagetable(pagetable);


  return 0;
}