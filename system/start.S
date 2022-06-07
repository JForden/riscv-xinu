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
    .func clear_bss
clear_bss:
	sw t0, 0(t2)
	addi t0, t0, 1
	bltu t0, t1, clear_bss
	.endfunc

	/* reset_handler: Reset handler routine executed to start up the kernel,
	 * when the processor is reset, or (currently) when an unhandled
	 * exception occurs.  */
    .func reset_handler
reset_handler:
    // Disable interrupts
    csrrsi x0, mstatus, RISCV_MSTATUS_MEI_BIT  
    // Clear bss
	//la t0, _bss
	//la t1, _end
	//li t2, 0
	//jal clear_bss

	la t1, _end
	li t3, 4096
	add a0, t1, t3

	/* Branch to nulluser function, located in xinu/system/initialize.c
	/* The function will initialize the system and become the null thread. */
	j	nulluser
	.endfunc