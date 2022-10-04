/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/**
 * Main Process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.  The default is designed to do something reasonable
 * on all platforms based on the devices and features configured.
 */
process main(void)
{
    kprintf("Hello, Xinu World, from hart %d!\r\n", gethartid());

    kprintf("Process completed!\r\n");

    return 0;
}
