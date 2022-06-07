/**
 * @file platforminit.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

#define MAX 256

/** 
 * Physical memory address at which the bootloader placed the ARM boot tags.
 * This is set by the code in start.S.  Here, initialize it to a dummy value to
 * prevent it from being placed in .bss.
 */
const struct atag *atags_ptr = (void *)-1;

/* End of kernel (used for sanity check)  */
extern void *_end;

/**
 * Initializes platform specific information for the QEMU RISC-V Virt
 * @return OK
 */
int platforminit(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    regptr->ier = 0;

    /* Enable UART FIFOs, clear and set interrupt trigger level       */
    regptr->fcr = UART_FCR_RRESET | UART_FCR_TRESET;
    regptr->fcr = UART_FCR_EFIFO;

    return OK;
}