#include <xinu.h>

int parseRsvMap(struct fdt_reserve_entry *rsv) {
    kprintf("Reserved System Areas:\r\n");
    while(rsv->address != 0 && rsv->size != 0) {
        kprintf("Address: 0x%016X\r\n", rsv->address);
        kprintf("Size: %d\r\n", rsv->size);
        rsv = rsv + 1;
    }
    kprintf("\r\n");
}

uint c2ioff(uint charoff) {
    return (charoff / 4) + ((charoff % 4) ? 1 : 0);
}

int parseDtbNodes(struct fdt_header *dtb_addr) {
    uint index = 0;
    char *name = NULL;
    uint* struct_base = (uint*)((uint)dtb_addr + be32_to_cpu(dtb_addr->off_dt_struct));
    uint* string_base = (uint*)(dtb_addr + be32_to_cpu(dtb_addr->off_dt_strings));

    while(1) {
        uint *tokptr =  struct_base + index;
        uint token = be32_to_cpu(*tokptr);

        switch (token)
        {
        case FDT_BEGIN_NODE:
            name = (char *)(tokptr + 1);
            kprintf("Node name: %s\r\n", name);
            index += 1 + c2ioff(1 + strlen((char*)(tokptr+1)));
            break;
        case FDT_PROP:
            struct fdt_prop *prop = (struct fdt_prop*)(tokptr+1);
            uint plen = be32_to_cpu(prop->len);
            char *pname = (char *)(string_base + be32_to_cpu(prop->nameoff));
            unsigned char *prop_val = ((unsigned char*)prop) + sizeof(struct fdt_prop);
            index += 1 + c2ioff(sizeof(struct fdt_prop) + plen);
            kprintf("  %s = %s\r\n", pname, prop_val);
            break;
        case FDT_END_NODE:
            name = NULL;
            index = index + 1;
            break;
        case FDT_NOP:
            index = index + 1;
            break;
        case FDT_END:
            return OK;
        default:
            kprintf("ERROR: Unknown device tree token 0x%08X\r\n", token);
            return SYSERR;
        }
    }
}

int parseDtb(void) {
    struct fdt_header* header = (struct fdt_header *)dtb_addr;

    if(FDT_MAGIC != cpu_to_be32(header->magic)){
        kprintf("ERROR: Invalid Device Tree!");
        return SYSERR;
    }

    parseRsvMap((struct fdt_reserve_entry *)((ulong)dtb_addr + be32_to_cpu(header->off_mem_rsvmap)));
    parseDtbNodes(header);

    kprintf("MAGIC: 0x%08X\r\n", cpu_to_be32(header->magic));
}