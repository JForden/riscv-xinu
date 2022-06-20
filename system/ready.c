/**
 * @file ready.c
 * @provides ready
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Make a process eligible for CPU service.
 * @param pid process id of the process to move to the ready state
 * @param resch if TRUE, reschedule will be called
 * @return OK if the process has been added to the ready list, else SYSERR
 */
syscall ready(pid_typ pid, bool resch)
{
    register pcb *ppcb;
    ASSERT(!isbadpid(pid));

    uint hart = gethartid();

    ppcb = &proctab[pid];
    ppcb->state = PRREADY;
    
    if (-1 == ppcb->core)
	{
		ppcb->core = hart;
	}
    
    enqueue(pid, readylist[ppcb->core][ppcb->priority]);
    
    if (resch)
    {
        resched();
    }
    return OK;
}