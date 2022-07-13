/**
 * @file safeInit
 * Initialize the memory protection subsystem.
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

struct pgmemblk *pgfreelist = NULL;      /**< system page table address */
uint pgtbl_nents = 0;               /**< number of entries in page table */

/**
 * Initialize the memory protection subsystem.  Allocates needed memory and
 * configures initial values for page tables.
 */
void safeInit(void)
{
    // This function should set up initial page table.

    /* number of pages in memory */
    pgtbl_nents = roundpage((ulong)platform.maxaddr - (ulong)memheap) / PAGE_SIZE;

    /* allocate memory system page table */
    //pgtbl = (struct pgtblent *)memget(nbytes);

    /* clear page table */
    //bzero(pgtbl, 4096);
    pgfreerange(memheap, platform.maxaddr);
}

int pgfreerange(void *start, void* end) {
    if(end > platform.maxaddr || (char *)start < (char *)memheap || (char *)end < (char *)start) {
        return SYSERR;
    }

    char *pgstart = (char *)roundpage(start);
    for(; pgstart + PAGE_SIZE < (char *)end; pgstart += PAGE_SIZE){
        if(SYSERR == pgfree(pgstart))
            return SYSERR;
    }

    return OK;
}

int pgfree(void *addr){
    struct pgmemblk* page;

    if ((ulong)addr % PAGE_SIZE != 0)
        return SYSERR;

    // Clears the data in the page
    bzero((char *)addr, PAGE_SIZE);

    //TODO: implement spinlock
    page = (struct pgmemblk *)addr;
    page->next=pgfreelist;
    pgfreelist = page;
    //TODO: Close spinlock

    return OK;
}

void *pgalloc(void){
    struct pgmemblk* page;

    if(pgfreelist == NULL) 
        return (void *)SYSERR;

    //TODO: implement spinlock
    page = pgfreelist;
    pgfreelist = page->next;

    // Clears the data in the page
    bzero((char *)page, PAGE_SIZE);

    return (void*)page;
}