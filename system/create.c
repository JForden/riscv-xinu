/**
 * @file create.c
 * @provides create, newpid, userret
 *
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <hart.h>
#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * @ingroup process
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param priority The priority given to the process(high,medium,low)
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong priority, char *name, ulong nargs, ...)
{
    ulong *saddr, *top;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    void INITRET(void);

    saddr = (ulong *)pgalloc();
    /* round up to even boundary    */
    pid = newpid();
    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc = numproc + 1;

    ppcb = &proctab[pid];
    /* setup PCB entry for new proc */
    ppcb->state = PRSUSP;       //  NOTE: concurrent newpid() sets state already.

    ppcb->stkbase = saddr;
    ppcb->stklen = PAGE_SIZE;
    strncpy(ppcb->name, name, PNMLEN);
    ppcb->core = -1;            // this will be set in ready()
    ppcb->priority = priority;
    ppcb->pagetable = vmcreate(pid, saddr);

    /* Initialize stack with accounting block. */
    saddr = (ulong *)((ulong)saddr + PAGE_SIZE);
    top = saddr;
    *saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / 8) * 8;
    }

    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

    va_start(ap, nargs);

    if (nargs)
    {
        for (i = 0; i < nargs; i++)
        {
            if(i <= 7){
			    ppcb->swaparea[i] = va_arg(ap, ulong);
		    } else {
			    //Add to stack
			    *(saddr + (i - 8)) = va_arg(ap, ulong);
		    }
        }
    }

    ppcb->swaparea[PREG_PC] = (ulong)funcaddr;
    ppcb->swaparea[PREG_RA] = (ulong)userret;
    ppcb->swaparea[PREG_SP] = (ulong)(PROCSTACKADDR + PAGE_SIZE - ((ulong)top - (ulong)saddr));

	ppcb->swaparea[PREG_KERNSATP] = (ulong)MAKE_SATP(0, _kernpgtbl);
	ppcb->swaparea[PREG_KERNSP] = (ulong)memheap;
	ppcb->swaparea[PREG_DISPATCH_ADDR] = (ulong)&dispatch;

    va_end(ap);

    // XXX
    return pid;
}

/**
 * @ingroup process
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */

    for (pid = NCORES; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        if (_atomic_compareAndSwapWeak
            (&(proctab[pid].state), PRFREE, PRSUSP))
            return pid;
    }
    return SYSERR;
}

/**
 * @ingroup process
 * Entered when a process exits by return.
 */
void userret(void)
{
    uint cpuid = gethartid();
    user_kill(currpid[cpuid]);
}
