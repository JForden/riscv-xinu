/**
 * @file pthread.c
 * Internal Xinu functions implementing system calls for PThread API.
 */
/* Embedded Xinu, Copyright (C) 2009, 2022.  All rights reserved. */

#include <xinu.h>

/* for syscall argument parsing */
#define SCARG(type, args)  (type)(*args++)

/* 
 * syscall pthread_create(pthread_t *thread, pthread_attr_t *attr,
 *               void *(*start_routine)(void *), void *arg);
 */
syscall sc_create(ulong *args)
{
    pthread_t *thread = SCARG(pthread_t *, args);
    pthread_attr_t *attr = SCARG(pthread_attr_t *, args);       /* ignore */
    void *start_routine = SCARG(void *, args);
    void *arg = SCARG(void *, args);
    char name[PNMLEN];
    static int ptnum = 0;

    kprintf("Test");
    //sprintf(name, "PThread%d", ptnum++);

    *thread = create(start_routine, INITSTK, INITPRIO, name, 1, arg);
    ready(*thread, RESCHED_YES);
    return OK;
}


/* 
 * syscall pthread_join(pthread_t thread, void **retval);
 */
syscall sc_join(ulong *args)
{
    pthread_t thread = SCARG(pthread_t, args);
    void **retval = SCARG(void **, args);       /* ignore */

    int ps;
    register pcb *ppcb;

    ps = disable();
    if (isbadpid(thread))
    {
        enable();
        return OK;
    }
    ppcb = &proctab[currpid[gethartid()]];
    ppcb->state = PRJOIN;
    enqueue(currpid[gethartid()], proctab[thread].joinq);
    resched();
    enable();
    return OK;
}

/*
 * syscall pthread_mutex_lock(pthread_mutex_t *mutex);
 */
syscall sc_lock(ulong *args)
{
    volatile pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);

    while (FALSE == _atomic_compareAndSwapStrong(mutex,
                                                 PTHREAD_MUTEX_UNLOCKED,
                                                 PTHREAD_MUTEX_LOCKED))
    {
        resched();
    }
    return OK;
}

/*
 * syscall pthread_mutex_unlock(pthread_mutex_t *mutex);
 */
syscall sc_unlock(ulong *args)
{
    pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);

    *mutex = PTHREAD_MUTEX_UNLOCKED;
    return OK;
}
