#include <xinu.h>

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;
    uchar c;

    /* Wait until a character is ready to be received.  */
    while (!(regptr->lsr & UART_LSR_DR))
    {
        /* Do nothing */
    }

    /* Get the next character from the UART by reading it from the Receiving
     * Holding Register.  */
    c = regptr->rbr;
    return c;
}

syscall kputc(uchar c)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    /* Wait until UART is ready for another character  */
    while ((regptr->lsr & UART_LSR_TEMT) != UART_LSR_TEMT)
    {
        /* Do nothing */
    }

    /* Send character. */
    regptr->buffer = c;
    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    lock_acquire(serial_lock);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    lock_release(serial_lock);
    va_end(ap);
    return retval;
}