/**
 * @file platforminit.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

#define MAX 256

/* End of kernel (used for sanity check)  */
extern void *_end;

/**
 * Initializes platform specific information for the QEMU RISC-V Virt
 * @return OK
 */
int platforminit(void)
{
    extern spinlock_t serial_lock;
    serial_lock = lock_create();

    parseDtb();

    //set_extensions();

    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    regptr->ier = 0;

    /* Enable UART FIFOs, clear and set interrupt trigger level       */
    regptr->fcr = UART_FCR_RRESET | UART_FCR_TRESET;
    regptr->fcr = UART_FCR_EFIFO;

    return OK;
}
