/**
 * @file platforminit.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

#define MAX 256

/* End of kernel (used for sanity check)  */
extern void *_end;

void set_extensions(){
    char ext[] = {'A', '-', 'C', 'D', 'E', 'F', '-', 'H', 'I', '-', '-', '-', 'M', '-', '-', '-', 'Q', '-', 'S', '-', 'U', '-', '-', '-', '-', '-'};
    int cindex = 0;
    unsigned long misa = getmisa();
    long arch_second_bit = (signed long)misa << 1;

    if ((signed long)misa < 0) {
        if (arch_second_bit < 0) {
            platform.architecture = 128;
        } else {
            platform.architecture = 64;
        }
    } else {
        if (arch_second_bit < 0) {
            platform.architecture = 32;
        } else {
            platform.architecture = 0;
        }
    }

    for (int i = 0; i < 26; i++){
        ulong shift = misa >> i;
        if (shift & 0x01) {
            platform.extensions[cindex] = ext[i];
            cindex++;
        }
    }
    platform.extensions[cindex] = '\0';

    kprintf("\n");
}

/**
 * Initializes platform specific information for the QEMU RISC-V Virt
 * @return OK
 */
int platforminit(void)
{
    extern spinlock_t serial_lock;
    serial_lock = lock_create();

    strlcpy(platform.manufacturer, "QEMU", PLT_STRMAX);
    strlcpy(platform.family, "Virt", PLT_STRMAX);
    platform.revision = 7;

    platform.maxaddr = (void *)((ulong)QEMU_START_ADDR + 0x8000000);
    platform.minaddr = (void *)((ulong)QEMU_START_ADDR);

    //set_extensions();

    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    regptr->ier = 0;

    /* Enable UART FIFOs, clear and set interrupt trigger level       */
    regptr->fcr = UART_FCR_RRESET | UART_FCR_TRESET;
    regptr->fcr = UART_FCR_EFIFO;

    return OK;
}