/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    irqmask im;
    ulong top;
    int n;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)memheap)
        || ((ulong)memptr > (ulong)platform.maxaddr))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* determine which freelist to use */
    for (n = 0; n < NCORES; n++)
    {
        if (((ulong)memptr >= (ulong)freelist[n].base)
            && ((ulong)memptr < (freelist[n].base + freelist[n].bound)))
        {
            break;
        }
    }

    im = disable();

    lock_acquire(freelist[n].memlock);

    prev = (memblk *) &freelist[n];
    next = freelist[n].head;
    while ((next != NULL) && (next < block))
    {
        prev = next;
        next = next->next;
    }

    /* find top of previous memblock */
    if ((ulong)prev == (ulong)&freelist[n])
    {
        top = NULL;
    }
    else
    {
        top = (ulong)prev + prev->length;
    }

    /* make sure block is not overlapping on prev or next blocks */
    if ((top > (ulong)block)
        || ((next != NULL) && ((ulong)block + nbytes) > (ulong)next))
    {
        lock_release(freelist[n].memlock);
        restore(im);
        return SYSERR;
    }

    freelist[n].length += nbytes;

    /* coalesce with previous block if adjacent */
    if (top == (ulong)block)
    {
        prev->length += nbytes;
        block = prev;
    }
    else
    {
        block->next = next;
        block->length = nbytes;
        prev->next = block;
    }

    /* coalesce with next block if adjacent */
    if (((ulong)block + block->length) == (ulong)next)
    {
        block->length += next->length;
        block->next = next->next;
    }

    lock_release(freelist[n].memlock);
    restore(im);
    return OK;
}
