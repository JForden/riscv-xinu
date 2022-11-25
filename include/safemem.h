

/**
 * @file safemem.h
 * Definitions for memory protection, including region-based memory
 * allocator, user thread memory allocator, and paging functions.
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#ifndef _SAFEMEM_H_
#define _SAFEMEM_H_

#include <stddef.h>

#define PAGE_SIZE 4096

/** Round a memory address to the upper page boundary */
#define roundpage(x) ((4095 + (ulong)(x)) & ~0x0FFF)
/** Truncate a memory address to the lower page boundary */
#define truncpage(x) ((ulong)(x) & ~0x0FFF)

/* Region allocator */

/**
 * Structure for a memory regions (collection of page sized memory chunks).
 */
struct memregion
{
    struct memregion *prev;     /**< pointer to previous region in list */
    struct memregion *next;     /**< pointer to next region in list */
    void *start;                /**< Starting address (page aligned) */
    uint length;                /**< Size of region */
};

extern struct memregion *regfreelist;  /**< List of free regions */
extern struct memregion *regalloclist; /**< List of allocated regions */
extern struct memregion *regtab;       /**< Array of regions */

/* Prototypes for memory region allocator */
void memRegionInit(void *, uint);
void memRegionClear(struct memregion *);
void memRegionInsert(struct memregion *, struct memregion **);
void memRegionRemove(struct memregion *, struct memregion **);
struct memregion *memRegionAlloc(uint);
struct memregion *memRegionSplit(struct memregion *, uint length);
struct memregion *memRegionValid(void *);

/**
 * Page table entry
 */
struct pgmemblk
{
    struct pgmemblk *next;
};

extern struct pgmemblk *pgfreelist;      /**< system page table */
extern uint pgtbl_nents;            /**< number of pages in page table */

typedef ulong *pgtbl;

#define PTE_V 1
#define PTE_R 1 << 1
#define PTE_W 1 << 2
#define PTE_X 1 << 3
#define PTE_U 1 << 4
#define PTE_G 1 << 5
#define PTE_A 1 << 6
#define PTE_D 1 << 7


//SATP Register
// +------+--------------------------+----------------------+
// | MODE | Address Space Identifier | Physical Page Number |
// +------+--------------------------+----------------------+
// |    4 |                       16 |                   44 |
// +------+--------------------------+----------------------+


//This sets the SATP registers's mode to 1000(8 in decimal)
//This enables Page-based 39-bit virtual addressing
#define SATP_SV39_ON (0x8L << 60)
//This disables SATP by setting mode to 0000(0 in decimal)
#define SATP_SV39_OFF (0x0L <<60)

#define MAKE_SATP(asid, pagetable) (SATP_SV39_ON | ((ulong)asid << 44) | (((ulong)pagetable) >> 12))

#define PTE2PA(pte)  ((pte >> 10) * PAGE_SIZE)  // Remove the first 10 bits (the flags and RSW).  Then multiply it by 4096 (page size)
#define PA2PTE(pa)   (((ulong)pa / PAGE_SIZE) << 10)    // Opposite of PTE2PA. Divide by the page size and then make room for flags
#define VAOFFSET 12             //Every virtual address is offset by 12 bits
#define PXMASK 0x1FF            //the virtual addr is masked into 3 sections of 9 bits. these correlate to lvl2,1,0
#define PXSHIFT(level)  (VAOFFSET+(9*(level)))
#define PX(level, addr) ((((ulong) (addr)) >> PXSHIFT(level)) & PXMASK)

/* Prototypes for memory protection functions */
void safeInit(void);
int safeMap(void *, short);
int safeMapRange(void *, uint, short);
int safeUnmap(void *);
int safeUnmapRange(void *, uint);
void safeKmapInit(void);

int pgfree(void *);
void *pgalloc(void);
int pgfreerange(void *start, void *end);
int mapAddress(pgtbl pagetable, ulong virtualaddr, ulong physicaladdr,
               ulong length, int attr);
int mapPage(pgtbl pagetable, pgtbl page, ulong virtualaddr, int attr);
ulong *pgTraverseAndCreate(pgtbl pagetable, ulong virtualaddr);
void printPageTable(pgtbl pagetable, int spaces);
pgtbl vmcreate(int pid, pgtbl stack);

#endif                          /* _SAFEMEM_H_ */
