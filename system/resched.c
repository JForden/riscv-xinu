/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
	int highest_prio;
	irqmask im;
	pcb *oldproc;               /* pointer to old process entry */
	pcb *newproc;               /* pointer to new process entry */

	uint cpuid = getcpuid();

	oldproc = &proctab[currpid[cpuid]];

	im = disable();

#if AGING
	// Decrement promote_medium quanta.
	// If it gets down to 0, promote a medium
	// priority process to a high priority.
	if (--promote_medium[cpuid] <= 0)
	{
		// move process from medium priority queue to a higher priority one,
		// if available.
		if (nonempty(readylist[cpuid][PRIORITY_MED]))
		{
			enqueue(dequeue(readylist[cpuid][PRIORITY_MED]), readylist[cpuid][PRIORITY_HIGH]);
		}
		
		// Decrement promote_low quanta,
		// if it gets down to 0, promote a low
		// priority process to a medium priority.
		if (--promote_low[cpuid] <= 0)
		{
			// Move process from low priority to medium priority queue.
			if (nonempty(readylist[cpuid][PRIORITY_LOW]))
			{
				enqueue(dequeue(readylist[cpuid][PRIORITY_LOW]), readylist[cpuid][PRIORITY_MED]);
			}
			
			// Reset promote_low quantum
			promote_low[cpuid] = QUANTUM;
		}

		// Reset promote_medium quantum
		promote_medium[cpuid] = QUANTUM;
		
	}
#endif

	/* place current process at end of ready queue */
	if (PRCURR == oldproc->state)
	{
		oldproc->state = PRREADY;
		enqueue(currpid[cpuid], readylist[cpuid][oldproc->priority]);
	}

	/* remove first process in highest priority ready queue */
	// determine queue to pick from
	if (nonempty(readylist[cpuid][PRIORITY_HIGH]))
	{
		highest_prio = PRIORITY_HIGH;
	}
	else if (nonempty(readylist[cpuid][PRIORITY_MED]))
	{
		highest_prio = PRIORITY_MED;
	}
	else
	{
		highest_prio = PRIORITY_LOW;
	}

	currpid[cpuid] = dequeue(readylist[cpuid][highest_prio]);
	newproc = &proctab[currpid[cpuid]];
	newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
	preempt[cpuid] = QUANTUM;
#endif

	ctxsw(&oldproc->regs, &newproc->regs);

	/* The OLD process returns here when resumed. */
	restore(im);
	return OK;
}
