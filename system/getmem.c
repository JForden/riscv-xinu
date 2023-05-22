/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    uint cpuid;
    register memblk *prev, *curr, *leftover;
    irqmask im;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    cpuid = getcpuid();

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    im = disable();

    lock_acquire(freelist[cpuid].memlock);

    prev = (memblk *) &freelist[cpuid];
    curr = freelist[cpuid].head;
    while (curr != NULL)
    {
        if (curr->length == nbytes)
        {
            prev->next = curr->next;
            freelist[cpuid].length -= nbytes;

            lock_release(freelist[cpuid].memlock);
            restore(im);
            return (void *)(curr);
        }
        else if (curr->length > nbytes)
        {
            /* split block into two */
            leftover = (struct memblock *)((ulong)curr + nbytes);
            prev->next = leftover;
            leftover->next = curr->next;
            leftover->length = curr->length - nbytes;
            freelist[cpuid].length -= nbytes;

            lock_release(freelist[cpuid].memlock);
            restore(im);
            return (void *)(curr);
        }
        prev = curr;
        curr = curr->next;
    }

    lock_release(freelist[cpuid].memlock);
    restore(im);
    return (void *)SYSERR;
}
