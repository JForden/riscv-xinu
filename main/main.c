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

/**
 * Main Process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.  The default is designed to do something reasonable
 * on all platforms based on the devices and features configured.
 */
process main(void)
{
    kprintf("Hello, Xinu World, from hart %d!\r\n", gethartid());

    int pid = 0;

    pid = create((void *)testbigargs, INITSTK, "MAIN1", 10,
                     0x1111111111111111UL, 0x2222222222222222UL, 0x3333333333333333UL, 0x4444444444444444UL,
                     0x5555555555555555UL, 0x6666666666666666UL, 0x7777777777777777UL, 0x8888888888888888UL, 
                     0x9999999999999999UL, 0x1010101010101010UL);
    kprintf("PID is %d!\r\n", pid);

    ready(pid, RESCHED_YES);
    kprintf("Process completed!\r\n");

    return 0;
}