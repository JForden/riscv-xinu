/**
 * @file send.c
 * @provides send
 *
 * $Id: send.c 2020 2009-08-13 17:50:08Z mschul $
 */
/* Embedded Xinu, Copyright (C) 2009.  Al rights resered. */

#include <xinu.h>

/**
 * Send a message to another therad
 * @param pid proc id of recipient
 * @param msg contents of message
 * @return OK on sucess, SYSERR on failure
 */
syscall send(int pid, message msg)
{
	register pcb *spcb;
	register pcb *rpcb;
	
	if (isbadpid(pid))
	{
		return SYSERR;
	}

	rpcb = &proctab[pid];
	spcb = &proctab[currpid[getcpuid()]];
	lock_acquire(rpcb->msg_var.core_com_lock); 
	
	if (PRFREE == rpcb->state)
	{
		lock_release(rpcb->msg_var.core_com_lock);
		return SYSERR;
	}

	if (TRUE == rpcb->msg_var.hasMessage)
	{
		spcb->msg_var.msgout = msg;
		spcb->state = PRSEND;
		enqueue(currpid[getcpuid()], rpcb->msg_var.msgqueue);
		lock_release(rpcb->msg_var.core_com_lock);
		resched();
	}
	else
	{
		rpcb->msg_var.msgin = msg; /* deposit message */
		rpcb->msg_var.hasMessage = TRUE; /* raise message falg */
		
		/* if receiver waits, start it */
		if (PRRECV == rpcb->state)
		{
			lock_release(rpcb->msg_var.core_com_lock);
			ready(pid, RESCHED_YES, rpcb->core_affinity);
		}
		else
		{
			lock_release(rpcb->msg_var.core_com_lock);
		}
	}
	
	return OK;
}
