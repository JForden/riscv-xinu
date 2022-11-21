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
    user_putc(0, 'H');
    user_putc(0, 'e');
    user_putc(0, 'l');
    user_putc(0, 'l');
    user_putc(0, 'o');
    user_putc(0, '!');
}