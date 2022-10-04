/**
 * @file elf.h
 */
/* Embedded Xinu, Copyright (C) 2022.  All rights reserved. */

#ifndef _ELF_H_
#define _ELF_H_

#define ELFCLASS64 2

#define EI_NIDENT 16
#define EI_MAG0   0
#define EI_MAG1   1
#define EI_MAG2   2
#define EI_MAG3   3
#define EI_CLASS  4
#define EI_DATA   5
#define EI_VERSION   6
#define EI_OSABI   7
#define EI_ABIVERSION   8
#define EI_PAD   9

extern const ulong _binary_data_elf_start[];

typedef ulong Elf64_Addr;
typedef ushort Elf64_Half;
typedef ulong Elf64_Off;
typedef uint Elf64_Word;
typedef ulong Elf64_Xword;

typedef struct
{
    uchar e_ident[EI_NIDENT];
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry;
    Elf64_Off e_phoff;
    Elf64_Off e_shoff;
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct
{
  Elf64_Word	p_type;			/* Segment type */
  Elf64_Word	p_flags;		/* Segment flags */
  Elf64_Off	p_offset;		/* Segment file offset */
  Elf64_Addr	p_vaddr;		/* Segment virtual address */
  Elf64_Addr	p_paddr;		/* Segment physical address */
  Elf64_Xword	p_filesz;		/* Segment size in file */
  Elf64_Xword	p_memsz;		/* Segment size in memory */
  Elf64_Xword	p_align;		/* Segment alignment */
} Elf64_Phdr;


#endif                          /* _ELF_H_ */
