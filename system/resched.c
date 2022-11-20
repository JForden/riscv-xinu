/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *, void *, ulong);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
void resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */
    int highest_prio;
    uint cpuid = gethartid();

    oldproc = &proctab[currpid[cpuid]];

    #if AGING
	// Decrement promote_medium quanta.
	// If it gets down to 0, promote a medium
	// priority process to a high priority.
	if (--promote_medium[cpuid] <= 0)
	{
		// move process from medium priority queue to a higher priority one,
		// if available.
		if (nonempty(readylist[cpuid][PRIO_MED]))
		{
			enqueue(dequeue(readylist[cpuid][PRIO_MED]), readylist[cpuid][PRIO_HIGH]);
		}
		
		// Decrement promote_low quanta,
		// if it gets down to 0, promote a low
		// priority process to a medium priority.
		if (--promote_low[cpuid] <= 0)
		{
			// Move process from low priority to medium priority queue.
			if (nonempty(readylist[cpuid][PRIO_LOW]))
			{
				enqueue(dequeue(readylist[cpuid][PRIO_LOW]), readylist[cpuid][PRIO_MED]);
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
		//kprintf("ADDING OLD PROC BACK\r\n");
		oldproc->state = PRREADY;
		//kprintf("HERE 1\r\n");
		enqueue(currpid[cpuid], readylist[cpuid][oldproc->priority]); 
		//kprintf("HERE 2\r\n");
	}

	/* remove first process in highest priority ready queue */
	// determine queue to pick from
	if (nonempty(readylist[cpuid][PRIO_HIGH]))
	{
		highest_prio = PRIO_HIGH;
	}
	else if (nonempty(readylist[cpuid][PRIO_MED]))
	{
		highest_prio = PRIO_MED;
	}
	else
	{
		highest_prio = PRIO_LOW;
	}

	//kprintf("Old process is %d\r\n", currpid[cpuid]);
	int i = 0;

	i = dequeue(readylist[cpuid][highest_prio]);
	//kprintf("SWITCHING from %d to %d", currpid[cpuid], i);
	currpid[cpuid] = i;
	newproc = &proctab[currpid[cpuid]];
	//kprintf("(%s)\r\n", newproc->name);
	newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
	preempt[cpuid] = QUANTUM;
#endif

    //ctxsw(&oldproc->regs, &newproc->regs, (void *)MAKE_SATP(currpid[cpuid], newproc->pagetable), USER_MODE);
	setpc(newproc->swaparea[PREG_PC]);
    
	ulong virt_trapret = INTERRUPTADDR + ((ulong)kernexit - (ulong)kernenter);
	kprintf("VIRT is 0x%08X\r\n", virt_trapret);
	((void (*)(ulong))virt_trapret)(MAKE_SATP(i, newproc->pagetable));

    /* The OLD process returns here when resumed. */
    //return OK;
}
