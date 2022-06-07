/**
 * @file start.S
 *
 * Initialization code for Embedded Xinu on the RISC-V Virt device.
 *
 * Embedded Xinu, Copyright (C) 2022.  All rights reserved.
 *
 */
#include <riscv.h>

.section .init
	.globl _start

	/* _start:  Entry point of the Xinu kernel.  This will be the very first
	/* byte of the kernel image and on the will be loaded at address 0x80000000.  */

	.func _start
_start:
    // Disable paging?

	/* Continue at reset handler. */
	
	j reset_handler
	.endfunc

.section .text

	/* reset_handler: Reset handler routine executed to start up the kernel,
	 * when the processor is reset, or (currently) when an unhandled
	 * exception occurs.  */
    .func reset_handler
reset_handler:
    // Disable interrupts
    csrrsi x0, mstatus, RISCV_MSTATUS_MEI_BIT  
    // Clear bss
	// Setup null stack

    // Debug
    ebreak

	/* Branch to nulluser function, located in xinu/system/initialize.c
	/* The function will initialize the system and become the null thread. */
	j	nulluser
	.endfunc