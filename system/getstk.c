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

pgtbl vmcreate() {
    pgtbl pagetable = pgalloc();

    // Map the following ranges
    // https://github.com/qemu/qemu/blob/master/hw/riscv/virt.c
    mapAddress(pagetable, UART_BASE, UART_BASE, 0x100, PTE_R | PTE_W | PTE_U);

    // Map kernel code
    mapAddress(pagetable, (ulong)&_start, (ulong)&_start, ((ulong)&_ctxsws - (ulong)&_start), PTE_R | PTE_W | PTE_X | PTE_U);

    // Map ctxsw
    mapAddress(pagetable, (ulong)&_ctxsws, CTXSWADDR, ((ulong)&_ctxswe - (ulong)&_ctxsws), PTE_R | PTE_X | PTE_G);

    // Map rest of kernel code
    mapAddress(pagetable, (ulong)&_ctxswe, (ulong)&_ctxswe, ((ulong)&_datas - (ulong)&_ctxswe), PTE_R | PTE_X | PTE_W | PTE_U);

    // Map global kernel structures and stack
    mapAddress(pagetable, (ulong)&_datas, (ulong)&_datas, ((ulong)memheap - (ulong)&_datas), PTE_R | PTE_W | PTE_U);

    // Map entirety of RAM
    kprintf("Mapping all of RAM\r\n");
    kprintf("CTXSW is at 0x%08X\r\n", &_ctxsws);
    mapAddress(pagetable, (ulong)memheap, (ulong)memheap, ((ulong)platform.maxaddr - (ulong)memheap), PTE_R | PTE_W | PTE_U);

    return pagetable;
}

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