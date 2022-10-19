#include <xinu.h>

int parseRsvMap(struct fdt_reserve_entry *rsv) {
    kprintf("Reserved System Areas:\r\n");
    while(rsv->address != 0 && rsv->size != 0) {
        kprintf("Address: 0x%016X\r\n", rsv->address);
        kprintf("Size: %d\r\n", rsv->size);
        rsv = rsv + 1;
    }

    return OK;
}

uint c2ioff(uint charoff) {
    return (charoff / 4) + ((charoff % 4) ? 1 : 0);
}

ulong dtb_read_long(ulong* dataptr) {
    return (be32_to_cpu((*dataptr) >> 32) << 32 | be32_to_cpu((*dataptr) >> 32));
}

int memoryHandler(char *name, char *pname, char *prop_val, uint plen) {
    if(!strcmp(pname, "reg") && plen == 16){
        ulong* data = (ulong *)prop_val;
        ulong min = dtb_read_long(data);
        ulong max = min + dtb_read_long(data + 1);

        platform.minaddr = min;
        platform.maxaddr = max;
    }

    return OK;
}

int uartHandler(char *name, char *pname, char *prop_val, uint plen) {
    if(!strcmp(pname, "reg")){
        ulong* data = (ulong *)prop_val;
        ulong addr = dtb_read_long(data);
        platform.uart_addr = (struct ns16550_uart_csreg *)addr;

        return OK;
    } else if (!strcmp(pname, "clock-frequency")) {
        uint *clock_freq_ptr = (uint *)prop_val;
        platform.uart_clock = *clock_freq_ptr;

        return OK;
    }

    return OK;
}

int baseHandler(char *name, char *pname, char *prop_val, uint plen) {
    if(!strcmp(pname, "model")){

        char * manufacturer = strrchr(prop_val, ',');
        if(manufacturer != NULL) {
            // Remove the comma
            strncpy(platform.family, prop_val, manufacturer - prop_val);

            manufacturer++;
            strncpy(platform.manufacturer, manufacturer, PLT_STRMAX);
        } else {
            strncpy(platform.manufacturer, prop_val, PLT_STRMAX);
        }
    }

    return OK;
}

int handleProp(char *name, char *pname, char *prop_val, uint plen){
    if(!strncmp(name, "memory@", 7)) {
        return memoryHandler(name, pname, prop_val, plen);
    } else if (!strncmp(name, "uart@", 5)){
        return uartHandler(name, pname, prop_val, plen);
    } else if (strnlen(name, PLT_STRMAX) == 0) {
        return baseHandler(name, pname, prop_val, plen);
    }

    return SYSERR;
}

int parseDtbNodes(struct fdt_header *dtb_addr) {
    uint index = 0;
    char *name = NULL;
    uint* struct_base = (uint*)((uint)dtb_addr + be32_to_cpu(dtb_addr->off_dt_struct));
    void* string_base = (void*)((void *)dtb_addr + be32_to_cpu(dtb_addr->off_dt_strings));

    while(1) {
        uint *tokptr =  struct_base + index;
        uint token = be32_to_cpu(*tokptr);

        switch (token)
        {
        case FDT_BEGIN_NODE:
            name = (char *)(tokptr + 1);
            //kprintf("Node: %s\r\n", name);
            index += 1 + c2ioff(1 + strlen((char*)(tokptr+1)));
            break;
        case FDT_PROP:
            struct fdt_prop *prop = (struct fdt_prop*)(tokptr+1);
            uint plen = be32_to_cpu(prop->len);
            char *pname = (char *)(string_base + be32_to_cpu(prop->nameoff));
            char *prop_val = (((char *)prop) + sizeof(struct fdt_prop));
            index += 1 + c2ioff(sizeof(struct fdt_prop) + plen);
            handleProp(name, pname, prop_val, plen);
            //kprintf("  %s (%d) = %s\r\n", pname, plen, prop_val);
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

    return OK;
}