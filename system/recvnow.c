/**
 * @file recvnow.c
 * @provides recvnow
 *
 */
/* Embedded Xinu, Copyright (C) 2020.  All rights resered. */

#include <xinu.h>

/**
 * recvnow - return pre-stored message, or immediate error.
 * @return message or SYSERR
 */

message recvnow(void)
{
	register pcb *ppcb;
	message msg;

	ppcb = &proctab[currpid[getcpuid()]];
	
	lock_acquire(ppcb->msg_var.core_com_lock);	
	if (FALSE == ppcb->msg_var.hasMessage)
	{
		lock_release(ppcb->msg_var.core_com_lock);
		return SYSERR;
	} 
	
	msg = ppcb->msg_var.msgin; /*retrieve message*/
	ppcb->msg_var.hasMessage = FALSE; /*reset message flag*/
	lock_release(ppcb->msg_var.core_com_lock);
	return msg;
}
