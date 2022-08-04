#include <xinu.h>

uint32_t c2ioff(uint32_t charoff) {
    return (charoff / 4) + ((charoff % 4) ? 1 : 0);
}

int parseRsvmap(void *addr){
    struct fdt_reserve_entry* entry = (struct fdt_reserve_entry*)addr;
    
    while(entry->address != 0 && entry->size != 0){
        kprintf("RESERVED: 0x%08X\r\n", entry->address);
        kprintf("RESERVED SIZES: 0x%08X\r\n", entry->size);
    }

    return OK;
}

int parseNodes(void *struct_addr, void* strings_addr, void (*handler)(char *nname, char *pname, uchar *pval, uint plen)){
    
    uint *token = (uint *)struct_addr;
    char *nname = NULL; 
    char *pname = NULL;
    uchar *pval = NULL;
    uint plen = 0;
    struct fdt_prop *prop = NULL;
    uint len = 0;

    while (1){
        switch (be32_to_cpu(*token))
        {
        case FDT_BEGIN_NODE:
            /* code */
            nname = (char *)(token + 1);
            len += 1 + c2ioff(1 + strlen((char*)(token+1)));
            break;
        case FDT_PROP:
            prop = (struct fdt_prop *)(token + 1);
            pname = (char*)(strings_addr + be32_to_cpu(prop->nameoff));
            plen = be32_to_cpu(prop->len);
            pval = ((unsigned char*)prop) + sizeof(struct fdt_prop);
            kprintf("%s = %s\r\n", nname, pname);
            handler(nname, pname, pval, plen);

            len += 1 + c2ioff(sizeof(struct fdt_prop) + plen);
            kprintf("LEN %d\r\n", len);
            break;
        case FDT_END_NODE:
            nname = NULL;
            break;
        case FDT_NOP:
            len++;
            break;
        case FDT_END:
            return OK;
            break;
        default:
            kprintf("DTB ERROR: Unknown token %d\r\n", be32_to_cpu(*token));
            return SYSERR;
        }
        token = token + len;
        len = 0;
    }

    return OK;
}

int parseDtb(){
    struct fdt_header *header = (struct fdt_header*)dtb_addr;

    if(FDT_MAGIC != be32_to_cpu(header->magic))
        return SYSERR;
    if(FDT_VERSION != be32_to_cpu(header->version))
        return SYSERR;
    
    parseRsvmap((void *)(dtb_addr + be32_to_cpu(header->off_mem_rsvmap)));
    
    parseNodes((void *)(dtb_addr + be32_to_cpu(header->off_dt_struct)), (void *)(dtb_addr + be32_to_cpu(header->off_dt_strings)), generic_dtb_handler);

    return 0;
}