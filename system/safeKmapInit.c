#include <xinu.h>

/**
 * Initialize kernel mappings to include statically configured kernel
 * memory.  Includes kernel text, data, stack, memory region table, and 
 * page table table.
 */
void safeKmapInit(void)
{
    // Get a page to start a page table via pgalloc
    pgtbl pagetable = pgalloc();

    // Map the following ranges
    // https://github.com/qemu/qemu/blob/master/hw/riscv/virt.c
    mapAddress(pagetable, UART_BASE, UART_BASE, 0x100, PTE_R | PTE_W);

    // Map kernel code
    mapAddress(pagetable, (ulong)&_start, (ulong)&_start, ((ulong)&_end - (ulong)_start), PTE_R | PTE_X);

    // Map kernel stacks
    mapAddress(pagetable, UART_BASE, UART_BASE, 0x100, PTE_R | PTE_W);

}

int mapAddress(pgtbl pagetable, ulong virtualaddr, ulong physicaladdr, ulong length, int attr){
    struct pgmemblk* page;
    ulong addr, end;

    length = roundpage(length);
    addr = (ulong)truncpage(virtualaddr);
    end = addr + length;

    for (; addr < end; addr += PAGE_SIZE)
    {
        // Walk through page table
    }

    //TODO: checks if length is equal to a page. 

    return OK;
}

struct pgmemblk* pgtraverse(pgtbl pagetable,  ulong virtualaddr, ulong physicaladdr){
    struct pgmemblk* page;

    for(int level = 2; level > 0; level--){
        page = &pagetable[PX(level,virtualaddr)];
        if(page & PTE_V)
            //TODO: Why does xv6 shift (((page) >> 10) << 12)
            pagetable = (pgtbl)PTE2PA(*page);

    }
    return page;
}

