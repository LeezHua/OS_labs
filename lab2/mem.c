#include <stdlib.h>
#include <string.h>
#include "mem.h"

ppage mempool;          // 内存池

// 创建页表
pagetable_t make_pagetable(){
  pagetable_t pgtbl = (pagetable_t)malloc(VMPAGES * sizeof(pte_t));
  memset(pgtbl, 0, VMPAGES * sizeof(pte_t));
  return pgtbl;
}

// 释放页表
void free_pagetable(pagetable_t pagetable){
  if(pagetable)
    free(pagetable);
}

// 初始化mempool
void init_mempool(uint n){
  mempool = NULL;
  for(int i = 0; i < n; ++i){
    ppage pp   = (ppage)malloc(sizeof(struct ppage));
    pp->next   = mempool;
    pp->paddr  = i * PGSIZE;
    mempool    = pp;
  }
}

// 释放mempool
void free_mempool(){
  while(mempool){
    ppage pp = mempool->next;
    free(mempool);
    mempool = pp;
  }
}

// 分配一页物理内存，无法分配返回-1，否则返回物理页号
int alloc_page(){
  if(mempool == NULL)
    return -1;
  ppage pp = mempool;
  mempool  = pp->next;
  uint pa = pp->paddr;
  free(pp);
  return pa;
}

// 释放一页物理页
void free_page(uint pa){
  ppage pp  = (ppage)malloc(sizeof(struct ppage));
  pp->next  = mempool;
  pp->paddr = pa;
  mempool   = pp;
}