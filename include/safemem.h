

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
    struct pgmemblk* next;
};

extern struct pgmemblk *pgfreelist;      /**< system page table */
extern struct pgtblent *pgtbl;      /**< system page table */
extern uint pgtbl_nents;            /**< number of pages in page table */

/* Prototypes for memory protection functions */
void safeInit(void);
int safeMap(void *, short);
int safeMapRange(void *, uint, short);
int safeUnmap(void *);
int safeUnmapRange(void *, uint);
void safeKmapInit(void);

int pgfree(void *);
void *pgalloc(void);
int pgfreerange(void *start, void* end);

#endif                          /* _SAFEMEM_H_ */