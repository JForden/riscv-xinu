/**
 * @file proc.h
 * @provides isbadpid
 *
 * COSC 3250 / COEN 4830 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2010.  All rights reserved. */

#ifndef _PROC_H_
#define _PROC_H_

#include <hart.h>
#include <safemem.h>

/* process table declarations and defined constants                      */

#ifndef NPROC                   /*   if number of processes is not set,    */
#define NPROC       50        /**< set the number of processes           */
#endif

/* unusual value marks the top of the process stack                      */
#define STACKMAGIC 0x0A0AAA99

/* process state constants                                               */

#define PRFREE      0       /**< process slot is free                    */
#define PRCURR      1       /**< process is currently running            */
#define PRSUSP      2       /**< process is suspended                    */
#define PRREADY     3       /**< process is on ready queue               */
#define PRJOIN      4       /**< process is on a join queue              */
#define PRWAIT      5       /**< process is waiting on semaphore         */

#define PRIO_LOW	0       /**< low process priority		*/
#define PRIO_MED	1       /**< medium process priority		*/
#define PRIO_HIGH	2       /**< high process priority		*/

/* miscellaneous process definitions                                     */

#define PNMLEN      16      /**< length of process "name"                */

/* the null process is always eligible to run                            */
#define NULLPROC    0       /**< id of the null process                  */
#define NULLPROC1   1
#define NULLPROC2   2
#define NULLPROC3   3
#define BADPID     (-1)     /**< used when invalid pid needed            */

/* Reschedule constants for ready  */
#define RESCHED_YES 1           /**< tell ready to reschedule           */
#define RESCHED_NO  0           /**< tell ready not to reschedule       */

/** 
 * Check for invalid process ids.  Note that interrupts must be disabled
 * for the condition to hold true between statements.
 */
#define isbadpid(x) ((x)<0 || (x)>=NPROC || PRFREE == proctab[(x)].state)

/* process table entry */

typedef struct pentry
{
    int state;           /**< process state: PRCURR, etc.             */
    int priority;        /**< priority of process                     */
    void *stkbase;       /**< base of run time stack                  */
    int stklen;          /**< stack length                            */
    int core;            /**< core affinity                           */
    uint joinq;          /**< JoinQ stuff                             */
    char name[PNMLEN];   /**< process name                            */
    pgtbl pagetable;
    ulong privilege;
    ulong regs[PREGS];     /**< stored process registers                */
} pcb;


#define PRIO_LOW	0       /**< low process priority		*/
#define PRIO_MED	1       /**< medium process priority		*/
#define PRIO_HIGH	2       /**< high process priority		*/

/* process initialization constants */
#define INITSTK  65536      /**< initial process stack size              */
#define INITPRIO PRIO_LOW   /**< initial process priority                */
#define INITRET  userret    /**< processes return address                */
#define MINSTK   4096       /**< minimum process stack size              */
#define NULLSTK  MINSTK     /**< null process stack size                 */

#define USER_MODE 0L
#define SUP_MODE  1L << 8

extern struct pentry proctab[];
extern int numproc;         /**< currently active processes              */
extern int currpid[];         /**< currently executing process             */

#endif                          /* _PROC_H_ */
