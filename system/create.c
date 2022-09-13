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
 * @param ssize    stack size in bytes
 * @param priority The priority given to the process(high,medium,low)
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, ulong priority, char *name,
               ulong nargs, ...)
{
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    void INITRET(void);

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
    pgtbl saddr = pgalloc();
    pgtbl pagetable = pgalloc();

    saddr = (ulong)saddr + PAGE_SIZE;
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
    ppcb->state = PRSUSP;//  NOTE: concurrent newpid() sets state already.

    ppcb->stkbase = saddr;
    ppcb->stklen = ssize;
    strncpy(ppcb->name, name, PNMLEN);
    ppcb->core = -1;            // this will be set in ready()
    ppcb->pagetable = pagetable;
    ppcb->priority = priority;

    /* Initialize stack with accounting block. */
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
			    ppcb->regs[i] = va_arg(ap, ulong);
		    } else {
			    //Add to stack
			    *(saddr + (i - 8)) = va_arg(ap, ulong);
		    }
        }
    }

    vmcreate(pagetable, saddr);     /* allocate new stack and pid   */

    ppcb->regs[PREG_PC] = (ulong)funcaddr;
    ppcb->regs[PREG_RA] = (ulong)userret;
    ppcb->regs[PREG_SPP] = (ulong)RISCV_SPP_TO_U_MODE;
    ppcb->regs[PREG_SP] = (ulong)0x300000500;

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
        if (_atomic_compareAndSwapWeak(&(proctab[pid].state), PRFREE, PRSUSP))
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
    kill(currpid[cpuid]);
}
