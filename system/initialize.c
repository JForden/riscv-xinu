#include <ns16550.h>

void nulluser(void)
{
    // Disable interrupts
    WriteUARTReg(UART_IER, 0);

    // Clear TX FIFO
    WriteUARTReg(UART_FCR, UART_FCR_TX_FIFO_RESET);
    // Clear RX FIFO
    WriteUARTReg(UART_FCR, UART_FCR_RX_FIFO_RESET);
    // Enable FIFOs
    WriteUARTReg(UART_FCR, UART_FCR_FIFO_ENABLE);

    WriteUARTReg(UART_THR, 'H');
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