/**
 * @file free.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup libxc
 *
 * Attempt to free a block of memory based on malloc() accounting information
 * stored in preceding two words.
 *
 * @param ptr
 *      A pointer to the memory block to free.
 */
syscall free(void *ptr)
{
    struct memblock *block;

    /* block points at the memblock we want to free */
    block = (struct memblock *)ptr;

    /* back up to accounting information */
    block--;

    /* don't memfree if we fail basic checks */
    if (block->next != block)
    {
        return SYSERR;
    }

    freemem(block, block->length);
    return OK;
}
