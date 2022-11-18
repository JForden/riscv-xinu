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
process main(int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten)
{
    kprintf("MAIN\r\n");
    kprintf("1: %d\r\n", one);
    kprintf("2: %d\r\n", two);
    kprintf("3: %d\r\n", three);
    kprintf("4: %d\r\n", four);
    kprintf("5: %d\r\n", five);
    kprintf("6: %d\r\n", six);
    kprintf("7: %d\r\n", seven);
    kprintf("8: %d\r\n", eight);
    kprintf("9: %d\r\n", nine);
    kprintf("10: %d\r\n", ten);

    kprintf("MAXADDR: 0x%16lX\r\n", MAXVIRTADDR);
    kprintf("INT S: 0x%016lX\r\n", (ulong)&_interrupts);
    kprintf("INT E: 0x%016lX\r\n", (ulong)&_interrupte);
}