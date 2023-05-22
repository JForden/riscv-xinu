/**
 * @file recv.c
 * @provides recv.
 *
 * $Id: receive.c 2020 2009-08-13 17:50:08Z mschul $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * recv - wait for a mesage and return it
 * @return message
 */

message recv(void)
{
	register pcb *ppcb;
	int senderpid = -1;
	message msg;
	ppcb = &proctab[currpid[getcpuid()]];
	
	lock_acquire(ppcb->msg_var.core_com_lock);
	
	if (FALSE == ppcb->msg_var.hasMessage)
	{
		ppcb->state = PRRECV;
		lock_release(ppcb->msg_var.core_com_lock);
		resched();
		lock_acquire(ppcb->msg_var.core_com_lock);
	}
	msg = ppcb->msg_var.msgin; //collect message
	
	if (nonempty(ppcb->msg_var.msgqueue)) //check for waiting send processes
	{
		senderpid = dequeue(ppcb->msg_var.msgqueue);
		ppcb->msg_var.msgin = proctab[senderpid].msg_var.msgout; //collect message
		proctab[senderpid].msg_var.msgout = EMPTY;
		lock_release(ppcb->msg_var.core_com_lock);
		ready(senderpid, RESCHED_NO, proctab[senderpid].core_affinity);	
	}
	else
	{
		ppcb->msg_var.hasMessage = FALSE; //reset flag
		lock_release(ppcb->msg_var.core_com_lock);
	}

	return msg;
}
