#include "vm.h"
#include <stdlib.h>
#include <string.h>

// 创建页表
pagetable_t make_pagetable(){
  pagetable_t pgtbl = (pagetable_t)malloc(VMPAGES * sizeof(pte_t));
  memset(pgtbl, 0, sizeof(pgtbl));
  return pgtbl;
}

// 释放页表
void free_pagetable(pagetable_t pagetable){
  if(pagetable)
    free(pagetable);
}