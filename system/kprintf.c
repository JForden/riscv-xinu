#include <xinu.h>
#include <serial.h>

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;
    uchar c;

    /* Wait until a character is ready to be received.  */
    //while (!(regptr->lsr & UART_LSR_DR))
    //{
        /* Do nothing */
    //}

    /* Get the next character from the UART by reading it from the Receiving
     * Holding Register.  */
    c = regptr->rbr;
    return c;
}

syscall kputc(uchar c)
{
    //WriteUARTReg(UART_THR, 'H');
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    /* Wait until UART is ready for another character  */
    //while ((regptr->lsr & UART_LSR_TEMT) != UART_LSR_TEMT)
    //{
        /* Do nothing */
    //}

    /* Send character. */
    //WriteUARTReg(UART_THR, 'H');
    //regptr->buffer = c;
    return 'A';
}