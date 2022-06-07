#ifndef  _NS16550_H_
#define  _NS16550_H_

#define UART_BASE  0x10000000L  // Found here https://github.com/qemu/qemu/blob/ca127b3fc247517ec7d4dad291f2c0f90602ce5b/hw/riscv/virt.c#L85

#define ReadUARTReg(reg) (*((volatile unsigned char *)(UART_BASE + reg)))
#define WriteUARTReg(reg, value) (*((volatile unsigned char *)(UART_BASE + reg)) = (value))

// UART registers
// Found here http://caro.su/msx/ocm_de1/16550.pdf (page 9)
#define UART_RBR    0           // Receiver Buffer Register (read-only)
#define UART_THR    0           // Transmitter Holding Register (write-only)
#define UART_IER    1           // Interrupt Enable Register (R/W)
#define UART_IIR    2           // Interrupt Identification Register (read-only)
#define UART_FCR    2           // FIFO Control Register (write-only)
#define UART_LCR    3           // Line Control Register (R/W)
#define UART_MCR    4           // Modem Control Register (R/W)
#define UART_LSR    5           // Line Status Register (read-only)
#define UART_MSR    6           // Modem Status Register (read-only)

#define UART_FCR_TX_FIFO_RESET 1<<2
#define UART_FCR_RX_FIFO_RESET 1<<1
#define UART_FCR_FIFO_ENABLE 1<<0

#endif                          /* _NS16550_H_ */