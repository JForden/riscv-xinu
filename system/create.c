/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <hart.h>
#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name,
               ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    void INITRET(void);

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc = numproc + 1;

    ppcb = &proctab[pid];
    /* setup PCB entry for new proc */
    ppcb->state = PRSUSP;//  NOTE: concurrent newpid() sets state already.

    ppcb->stkbase = saddr;
    ppcb->stklen = ssize;
    strncpy(ppcb->name, name, PNMLEN);
    ppcb->core = -1;            // this will be set in ready()

    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    va_start(ap, nargs);

    if (nargs)
    {
        for (i = 0; i < nargs; i++)
        {
            ppcb->regs[i] = va_arg(ap, int);
        }
    }

    ppcb->regs[PREG_PC] = (ulong)funcaddr;
    ppcb->regs[PREG_RA] = (ulong)userret;
    ppcb->regs[PREG_SP] = (ulong)saddr;

    va_end(ap);

    // XXX
    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    return 1;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    //kprintf("Killing %d!\r\n", currpid);
    kill(currpid);
}
