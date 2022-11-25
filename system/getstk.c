/**
 * @file getstk.c
 * @provides getstk
 *
 * $Id: getstk.c 190 2007-07-13 22:24:20Z agember $
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>
#include <platform.h>

extern void *end;

/**
 * Allocate stack memory, returning address of topmost word.
 * @param nbytes amount of memory to allocate, in bytes
 * @return address of the topmost word
 */

pgtbl vmcreate(int pid, pgtbl stack) {
	//kprintf("\r\n\r\nCreating virtual memory\r\n");
    pcb *ppcb = &proctab[pid];
    pgtbl pagetable = pgalloc();

    // Map kernel code
    mapAddress(pagetable, (ulong)&_start, (ulong)&_start, ((ulong)&_ctxsws - (ulong)&_start), PTE_R | PTE_X | PTE_U);

    // Map interrupt
    mapAddress(pagetable, INTERRUPTADDR, (ulong)&_interrupts, PAGE_SIZE, PTE_R | PTE_X | PTE_G);

    // Map rest of kernel code
    mapAddress(pagetable, (ulong)&_interrupte, (ulong)&_interrupte, ((ulong)&_datas - (ulong)&_interrupte), PTE_R | PTE_X | PTE_U);

    // Map global kernel structures and stack
    mapAddress(pagetable, (ulong)&_datas, (ulong)&_datas, ((ulong)memheap - (ulong)&_datas), PTE_R | PTE_U);

	// Map process stack
    mapPage(pagetable, stack, PROCSTACKADDR, PTE_R | PTE_W | PTE_U);

	// Map context swap area
	pgtbl swaparea = pgalloc();
	ppcb->swaparea = swaparea;
    mapPage(pagetable, swaparea, SWAPAREAADDR, PTE_R | PTE_W);

    //printPageTable(pagetable, 0);

    return pagetable;
}