#include <xinu.h>

/**
 * Initialize kernel mappings to include statically configured kernel
 * memory.  Includes kernel text, data, stack, memory region table, and 
 * page table table.
 */
void safeKmapInit(void)
{
    register pcb *ppcb;
    // Get a page to start a page table via pgalloc
    pgtbl pagetable = pgalloc();

    // Map the following ranges
    // https://github.com/qemu/qemu/blob/master/hw/riscv/virt.c
    mapAddress(pagetable, UART_BASE, UART_BASE, 0x100, PTE_R | PTE_W);

    // Map kernel code
    mapAddress(pagetable, (ulong)&_start, (ulong)&_start, ((ulong)&_ctxsws - (ulong)&_start), PTE_R | PTE_X );

    // Map interrupt
    mapAddress(pagetable, INTERRUPTADDR, (ulong)&_interrupts, PAGE_SIZE, PTE_R | PTE_X | PTE_G);
    
    // Map rest of kernel code
    mapAddress(pagetable, (ulong)&_interrupte, (ulong)&_interrupte, ((ulong)&_datas - (ulong)&_interrupte), PTE_R | PTE_X);

    // Map global kernel structures and stack
    mapAddress(pagetable, (ulong)&_datas, (ulong)&_datas, ((ulong)memheap - (ulong)&_datas), PTE_R | PTE_W);

    // Map entirety of RAM
    //kprintf("Mapping all of RAM\r\n");
    mapAddress(pagetable, (ulong)memheap, (ulong)memheap, ((ulong)platform.maxaddr - (ulong)memheap), PTE_R | PTE_W);

    ppcb = &proctab[currpid[gethartid()]];
    ppcb->pagetable = pagetable;

    _kernpgtbl = pagetable;
    //kprintf("Kernel pagetable is 0x%016lX\t\n", _kernpgtbl);

    set_satp(MAKE_SATP(0, pagetable));
}

int mapPage(pgtbl pagetable, pgtbl page, ulong virtualaddr, int attr){
    ulong *pte = NULL;
    ulong addr;

    addr = (ulong)truncpage(virtualaddr);

    if((pte = pgTraverseAndCreate(pagetable, addr)) == (ulong *)SYSERR){
        return SYSERR;
    }
    if(*pte & PTE_V) {
        kprintf("REMAPPED 0x%08X!!!\r\n", addr);
    }
    *pte = PA2PTE(page) | attr | PTE_V;

    return OK;
}

int mapAddress(pgtbl pagetable, ulong virtualaddr, ulong physicaladdr, ulong length, int attr){
    ulong *pte = NULL;
    ulong addr, end;
    ulong nlength;


    if(length==0){ 
        return SYSERR;
    }

    nlength = roundpage(length);
    addr = (ulong)truncpage(virtualaddr);
    end = addr + nlength;
    //kprintf("Mapping from 0x%016lX to 0x%016lX with physical address 0x%016lX\r\n", addr, end, physicaladdr);

    for (; addr < end; addr += PAGE_SIZE, physicaladdr += PAGE_SIZE)
    {
        if((pte = pgTraverseAndCreate(pagetable, addr)) == (ulong *)SYSERR){
            return SYSERR;
        }
        if(*pte & PTE_V) {
            kprintf("REMAPPED 0x%16X!!!\r\n", addr);
        }

        *pte = PA2PTE(physicaladdr) | attr | PTE_V;
    }

    return OK;
}

// Virtual Address:
// +--------+----+----+----+--------+
// | Extra  | L2 | L1 | L0 | Offset |
// +--------+----+----+----+--------+
// |     25 |  9 |  9 |  9 |     12 |
// +--------+----+----+----+--------+


//Sv39 Risc-V uses a three level page table structure. When mapping from a virtual address to a physical address the 9 bites that denote L2 should be looked at first.
//Risc-V identifies a leaf page table by its flags. 


//Page Table Entry
// +----------+----------------------+-----+---+---+---+---+---+---+---+---+
// | Reserved | Physical Page Number | RSW | D | A | G | U | X | W | R | V |
// +----------+----------------------+-----+---+---+---+---+---+---+---+---+
// | 63-54    | 53-10                | 9-8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
// +----------+----------------------+-----+---+---+---+---+---+---+---+---+

// Returns the PTE of the specified virtual address.  This will create pages along the way.
ulong *pgTraverseAndCreate(pgtbl pagetable,  ulong virtualaddr){
    ulong *pte = NULL;

    for(int level = 2; level > 0; level--){
        pte = &pagetable[PX(level, virtualaddr)];
        if(*pte & PTE_V){
            pagetable = (pgtbl)PTE2PA(*pte);
        }
        else {
            if((pagetable = (ulong *)pgalloc()) == (ulong *)SYSERR)
                return (ulong *)SYSERR;
            *pte = PA2PTE(pagetable) | PTE_V; 
        }
    }

    return &pagetable[PX(0, virtualaddr)];
}

static void printSpaces(int spaces) {
    for(int i = 0; i < spaces; i++) {
        kprintf(" ");
    }
}

void printPageTable(pgtbl pagetable, int spaces){
    printSpaces(spaces);
    kprintf("Table at 0x%08X:\r\n", (ulong)pagetable);
    for(int i = 0; i < 512; i++){
        if(pagetable[i] & PTE_V) {
            printSpaces(spaces + 2);
            if(!(pagetable[i] & (PTE_R | PTE_W | PTE_X))){
                kprintf("Link PTE (0x%016lX)| Address 0x%016lX\r\n", pagetable[i], (pgtbl)PTE2PA(pagetable[i]));
                printPageTable((pgtbl)PTE2PA(pagetable[i]), spaces + 2);
            } else {
                kprintf("Leaf PTE (0x%016lX)| Address 0x%016lX\r\n", pagetable[i], (pgtbl)PTE2PA(pagetable[i]));
            }
        }
    }
}
