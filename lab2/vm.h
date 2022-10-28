#include <stdlib.h>
#include <string.h>

typedef unsigned char  byte;
typedef unsigned int   uint;
typedef uint           pte_t;
typedef pte_t*         pagetable_t;

#define PGSIZE         1024
#define VMPAGES        32
#define PTE_E          (1 << 0)
#define PTE2PA(pte)    ((pte >> 10) << 10)
#define PA2PTE(pa)     ((pa >> 10) << 10)
#define PX(va)         ((va >> 10) & 0x1f)
#define INSTR2VA(n)    (((n / 10) << 10) | (n % 10 * 4))

// 创建页表
pagetable_t make_pagetable();

// 释放页表
void free_pagetable(pagetable_t pagetable);