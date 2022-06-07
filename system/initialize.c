#include <xinu.h>

void nulluser(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    regptr->ier = 0;

    /* Enable UART FIFOs, clear and set interrupt trigger level       */
    regptr->fcr = UART_FCR_RRESET | UART_FCR_TRESET;
    regptr->fcr = UART_FCR_EFIFO;
    //kputc('H');
    /*kputc('e');
    kputc('l');
    kputc('l');
    kputc('o');
    kputc(' ');
    kputc('W');
    kputc('o');
    kputc('r');
    kputc('l');
    kputc('d');
    kputc('!');*/

    // Disable interrupts
    //WriteUARTReg(UART_IER, 0);

    // Clear TX FIFO
    //WriteUARTReg(UART_FCR, UART_FCR_TX_FIFO_RESET);
    // Clear RX FIFO
    //WriteUARTReg(UART_FCR, UART_FCR_RX_FIFO_RESET);
    // Enable FIFOs
    //WriteUARTReg(UART_FCR, UART_FCR_FIFO_ENABLE);
    WriteUARTReg(UART_THR, 'e');
    WriteUARTReg(UART_THR, 'l');
    WriteUARTReg(UART_THR, 'l');
    WriteUARTReg(UART_THR, 'o');
    WriteUARTReg(UART_THR, ' ');
    WriteUARTReg(UART_THR, 'W');
    WriteUARTReg(UART_THR, 'o');
    WriteUARTReg(UART_THR, 'r');
    WriteUARTReg(UART_THR, 'l');
    WriteUARTReg(UART_THR, 'd');
    WriteUARTReg(UART_THR, '!');

    /* null process has nothing else to do but cannot exit  */
    while (1)
    {

    }
}