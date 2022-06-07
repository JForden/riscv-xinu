#include <xinu.h>

void nulluser(int bss, int end)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    regptr->ier = 0;

    /* Enable UART FIFOs, clear and set interrupt trigger level       */
    regptr->fcr = UART_FCR_RRESET | UART_FCR_TRESET;
    regptr->fcr = UART_FCR_EFIFO;
    kputc('H');
    kputc('e');
    kputc('l');
    kputc('l');
    kputc('o');
    kputc(' ');
    kputc('W');
    kputc('o');
    kputc('r');
    kputc('l');
    kputc('d');
    kputc('!');
    kputc('\n');
    kputc('T');

    /* null process has nothing else to do but cannot exit  */
    while (1)
    {

    }
}