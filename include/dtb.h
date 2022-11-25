/**
 * @file dtb.h
 *
 * Device tree definitions
 */
/* Embedded Xinu, Copyright (C) 2022.  All rights reserved. */

#ifndef _DBT_H_
#define _DBT_H_

#include <stdint.h>

#define FDT_MAGIC 0xd00dfeed
#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE 0x00000002
#define FDT_PROP 0x00000003
#define FDT_NOP 0x00000004
#define FDT_END 0x00000009


struct fdt_header
{
    uint magic;
    uint totalsize;
    uint off_dt_struct;
    uint off_dt_strings;
    uint off_mem_rsvmap;
    uint version;
    uint last_comp_version;
    uint boot_cpuid_phys;
    uint size_dt_strings;
    uint size_dt_struct;
};

struct fdt_reserve_entry
{
    ulong address;
    ulong size;
};

struct fdt_prop
{
    uint len;
    uint nameoff;
};

extern void *dtb_addr;

int parseDtb(void);

#endif
