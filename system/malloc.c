/**
 * @file malloc.c
 * This file is deprecated in favor of mem/malloc.c and the user heap
 * allocator.  However, it remains here for backup purposes.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup libxc
 *
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    struct memblock *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

    /* make room for accounting info */
    size += sizeof(struct memblock);

    /* acquire memory from kernel */
    pmem = (struct memblock *)getmem(size);
    if (SYSERR == (uint)pmem)
    {
        return NULL;
    }

    /* set accounting info */
    pmem->next = pmem;
    pmem->marked = 0;
    pmem->length = size;

    //TODO: add new memblock to gcList
    //usedList->next = pmem;


    return (void *)(pmem + 1);  /* +1 to skip accounting info */
}
