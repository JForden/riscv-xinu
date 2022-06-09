/**
 * @file kill.c
 * Provides: kill
 *
 * COSC 3250/ COEN 4820 Assignment 4
 */

/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/*
 * kill  --  kill a process and remove it from the system
 */
syscall kill(int pid)
{
    pcb *ppcb;                  /* points to process control block for pid */
    uint cpuid = gethartid();

    if (isbadpid(pid) || (0 == pid)
        || (cpuid != proctab[pid].core)
        || (PRFREE == (ppcb = &proctab[pid])->state))
    {
        kprintf("TEST\r\n");
        return SYSERR;
    }


    kprintf("IN HERE!!!!!!");

    ppcb = &proctab[pid];

    numproc = numproc - 1;

    if (numproc <= 4)
    {
        // all processes are done running
//              kprintf("\r\n\r\nAll user process have completed.\r\n\r\n");
//              extern void halt(void);
//              halt();
    }

    switch (ppcb->state)
    {
    case PRCURR:
        ppcb->state = PRFREE;   /* suicide */
        resched();
        // The process should never run this line after resched is called.
        break;
    case PRREADY:
        remove(pid);
        ppcb->state = PRFREE;
        break;
    default:
        ppcb->state = PRFREE;
        break;
    }

    return OK;
}
