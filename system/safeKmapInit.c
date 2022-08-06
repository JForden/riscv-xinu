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
    mapAddress(pagetable, (ulong)&_start, (ulong)&_start, ((ulong)_datas - (ulong)_start), PTE_R | PTE_X | PTE_W);

    // Map global kernel structures
    mapAddress(pagetable, (ulong)_datas, (ulong)_datas, ((ulong)&_end - (ulong)_datas), PTE_R | PTE_W);

    // Map kernel stack
    mapAddress(pagetable, (ulong)&_end, (ulong)&_end, ((ulong)memheap - (ulong)&_end), PTE_R | PTE_W);

    printPageTable(pagetable, 0);
    // Map entire address space
    mapAddress(pagetable, (ulong)memheap, (ulong)memheap, ((ulong)platform.maxaddr - (ulong)memheap), PTE_R | PTE_W);

    set_satp(MAKE_SATP(pagetable));
    kprintf("Test 123\r\n");
}

int mapAddress(pgtbl pagetable, ulong virtualaddr, ulong physicaladdr, ulong length, int attr){
    ulong *pte = NULL;
    ulong addr, end;


    if(length==0){ 
        return SYSERR;
    }

    length = roundpage(length);
    addr = (ulong)truncpage(virtualaddr);
    end = addr + length;
    

    for (; addr < end; addr += PAGE_SIZE, physicaladdr += PAGE_SIZE)
    {
        //kprintf("MAPPED 0x%08X\r\n", addr);
        if((pte = pgTraverseAndCreate(pagetable, addr)) == (ulong *)SYSERR){
            return SYSERR;
        }
        if(*pte & PTE_V) {
            kprintf("REMAPPED 0x%08X!!!\r\n", addr);
        }
        *pte = PA2PTE(physicaladdr) | attr | PTE_V;
    }

    return OK;
}

int mapVAddress(pgtbl pagetable, pgtbl toppage, ulong virtualaddr, ulong length, int attr){
    ulong *pte = NULL;
    ulong addr, end, numofpages;

    if(length==0){ 
        return SYSERR;
    }

    length = roundpage(length);
    addr = (ulong)truncpage(virtualaddr);
    end = addr + length;
    numofpages = length / PAGE_SIZE;
    
    for (int i = 0; addr < end; addr += PAGE_SIZE, i++)
    {
        pgtbl page = NULL;
        if(i != numofpages - 1)
            page = pgalloc();
        else
            page = toppage;
            
        if(page == SYSERR)
            return SYSERR;
        if((pte = pgTraverseAndCreate(pagetable, addr)) == (ulong *)SYSERR){
            return SYSERR;
        }
        *pte = PA2PTE(page) | attr | PTE_V;
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
        //kprintf("VAL is %d\r\n", PX(level, virtualaddr));
        //kprintf("PTE ADDR IS 0x%08X\r\n", &pagetable);
        pte = &pagetable[PX(level, virtualaddr)];
        //kprintf("HERE 5555\r\n");
        if(*pte & PTE_V){
            pagetable = (pgtbl)PTE2PA(*pte);
        }
        else {
            //kprintf("HERE 4444");
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
                kprintf("Link PTE (0x%08X)| Address 0x%08X\r\n", pagetable[i], (pgtbl)PTE2PA(pagetable[i]));
                printPageTable((pgtbl)PTE2PA(pagetable[i]), spaces + 2);
            } else {
                kprintf("Leaf PTE (0x%08X)| Address 0x%08X\r\n", pagetable[i], (pgtbl)PTE2PA(pagetable[i]));
            }
        }
    }
}
