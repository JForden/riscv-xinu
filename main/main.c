/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>


void testbigargs(ulong a, ulong b, ulong c, ulong d, ulong e, ulong f, ulong g, ulong h, ulong i, ulong j)
{
    // Test user_none
    kprintf("This is a test of ...");
    user_putc(0, 'h');
    kprintf("user_none() syscall\r\n");
}

void *jointhread(void *arg)
{
    pthread_t target = (int) arg;
    kprintf("Thread %d calling pthread_join(%d, NULL)\r\n", currpid, target);
    pthread_join(target, NULL);
    kprintf("Thread %d has awakened!\r\n", currpid);
    kprintf("Target thread %d has state %d [0]\r\n", 
		    target, proctab[target].state);
    return NULL;
}

void *joinee(void *arg)
{
	kprintf("Target thread %d\r\n", currpid);
	while (1)
		resched();
	return NULL;
}

/**
 * Main Process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.  The default is designed to do something reasonable
 * on all platforms based on the devices and features configured.
 */
process main(void)
{
    kprintf("Hello, Xinu World, from hart %d!\r\n", gethartid());

    int pid = 0;
    kprintf("PID is %d!\r\n", pid);

    char test;
	int rc = 0;

	pthread_t testida = -1;
	pthread_t testidb = -1;
	pthread_mutex_t mutex = PTHREAD_MUTEX_UNLOCKED;


    rc = pthread_create(&testida, NULL, joinee, (void *)0);
		resched();
		rc = pthread_create(&testidb, NULL, jointhread, 
				(void *)testida);
		rc = pthread_create(&testidb, NULL, jointhread, 
				(void *)testida);
		rc = pthread_create(&testidb, NULL, jointhread, 
				(void *)testida);
		resched();
		resched(); // Lottery scheduler.  Just yield a bunch.
		resched();
		resched();
		resched();
		kprintf("Killing target thread %d\r\n", testida);
		kill(testida);
		while (numproc > 2)
            resched();

    ready(pid, RESCHED_YES);
    kprintf("Process completed!\r\n");

    return 0;
}