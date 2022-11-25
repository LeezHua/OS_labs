#ifndef __MEM_H__
#define __MEM_H__

typedef unsigned char  byte;
typedef unsigned int   uint;
typedef uint           pte_t;
typedef pte_t*         pagetable_t;

#define PGSIZE         1024
#define VMPAGES        32
#define PTE_V          (1 << 0)
#define PTE2PA(pte)    ((pte >> 10) << 10)
#define PA2PTE(pa)     ((pa >> 10) << 10)
#define PX(va)         ((va >> 10) & 0x1f)
#define INSTR2VA(n)    (((n / 10) << 10) | (n % 10 * 4))

typedef struct ppage{
  uint paddr;
  struct ppage* next;
}*ppage;

// 创建页表
pagetable_t make_pagetable();

// 释放页表
void free_pagetable(pagetable_t pagetable);

// 初始化mempool
void init_mempool(uint n);

// 释放mempool
void free_mempool();

// 分配一页物理内存，无法分配返回-1，否则返回物理页地址
int alloc_page();

// 释放一页物理页
void free_page(uint ppn);

#endif