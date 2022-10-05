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
void *getstk(ulong nbytes)
{
    /* NOTE: This is a completely broken implementation of getstk(),      */
    /*  intended only for introductory assignments before implementing    */
    /*  proper dynamic heap allocation.                                   */

    ulong newstk;

    if (nbytes == 0)
    {
        return ((void *)SYSERR);
    }

    nbytes = (nbytes + 15) & ~0x0F;

    if ((ulong)platform.maxaddr - nbytes < (ulong)&_end)
    {
        return ((void *)SYSERR);
    }

    newstk = ((ulong)platform.maxaddr - 4) & 0xFFFFFFF0;
    platform.maxaddr = (char *)(((ulong)platform.maxaddr) - nbytes);

    return ((void *)newstk);
}

void *vmcreate(pgtbl pagetable){

    pgtbl page = pgalloc();
    // Map process stack
    //mapAddress(pagetable, 0x300000000, (ulong)page, 4096, PTE_R | PTE_W | PTE_X );
    //mapVAddress(pagetable, (ulong)page, 0x300000000, 4096, PTE_R | PTE_W | PTE_X | PTE_U);
    kprintf("HERE: vmcreate 1\r\n");

    //kprintf("HERE: vmcreate 2\r\n");
    // Map code
    mapAddress(pagetable, UART_BASE, UART_BASE, 0x100, PTE_R | PTE_W | PTE_X );

    // Map kernel code
    //kprintf("HERE: vmcreate 3\r\n");
    mapAddress(pagetable, (ulong)&_start, (ulong)&_start, ((ulong)_datas - (ulong)_start), PTE_R | PTE_W | PTE_X );
    //kprintf("HERE: vmcreate 4\r\n");

    // Map global kernel structures and stack
    mapAddress(pagetable, (ulong)_datas, (ulong)_datas, ((ulong)memheap - (ulong)_datas), PTE_R | PTE_X | PTE_W );
    //kprintf("HERE: vmcreate 5\r\n");

    return NULL;
}