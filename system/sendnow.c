/**
 * @file sendnow.c
 * @provides sendnow.
 *
 */
/* Embedded Xinu, Copyright (C) 2020.   All rights reserved. */

#include <xinu.h>

/**
 * Send a message to anoher thread
 * @param pid proc id of recipient
 * @param msg contents of message
 * @return OK on sucess, SYSERR on failure
 */

syscall sendnow(int pid, message msg)
{
	register pcb *ppcb;

	ppcb = &proctab[pid];
	lock_acquire(ppcb->msg_var.core_com_lock);

	if (isbadpid(pid))
	{
		lock_release(ppcb->msg_var.core_com_lock);
		return SYSERR;
	}

	if (PRFREE == ppcb->state || ppcb->msg_var.hasMessage)
	{
		lock_release(ppcb->msg_var.core_com_lock);
		return SYSERR;
	}

	ppcb->msg_var.msgin = msg; /* deposit message */
	ppcb->msg_var.hasMessage = TRUE; /* raise message flag */

	/* if receiver waits start it */
	if (PRRECV == ppcb->state)
	{
		lock_release(ppcb->msg_var.core_com_lock);
		ready(pid, RESCHED_NO, ppcb->core_affinity);
		return OK;
	}
	
	lock_release(ppcb->msg_var.core_com_lock);
	return OK;
}
