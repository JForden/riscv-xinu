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
	/* Continue at reset handler. */
	j reset_handler
	.endfunc

.section .text
    .func clear_bss
clear_bss: // block starting symbol
	sw t0, 0(t2) // store word, dereferences t0, stores into t2+0    (t0 is temp/alt link reg) (t2 is a temp reg)
	addi t0, t0, 1 // adds value of t0, to 1 and stores in t0
	bltu t0, t1, clear_bss //branch if less then, if t0>t1 
	.endfunc

	/* reset_handler: Reset handler routine executed to start up the kernel,
	 * when the processor is reset, or (currently) when an unhandled
	 * exception occurs.  */
    .func reset_handler
reset_handler:
    // Enables interrupts
    csrrsi x0, mstatus, RISCV_MSTATUS_MEI_BIT  
    // Clear bss
	//la a0, _bss
	//la a1, _end
	//li a2, 0
	//jal clear_bss

	// Loads address of the function interrupt in t1
	la t1, interrupt
	// Sets stvec to the address of interrupt
	csrrw x0, stvec, t1

	la a1, _end
	li a3, 8196
	add sp, a1, a3

	la a4, memheap
	sw sp, 0(a4)

	// Set the previous mode to S-Mode
	li t1, RISCV_MPP_TO_S_MODE
	csrrs x0, mstatus, t1

	// Tell the CPU to handle all exceptions in S-Mode 
	li t1, RISCV_MDELEG_ALL_S_MODE
	csrrs x0, medeleg, t1
	
	// Tell the CPU to handle all interrupts in S-Mode
	li t1, RISCV_MDELEG_ALL_S_MODE
	csrrs x0, mideleg, t1

	// Tell the CPU to handle all exceptions in S-Mode
	li t1, RISCV_ENABLE_ALL_SMODE_INTR
	csrrs x0, sie, t1

	// Allow S-Mode to access all of memory
	li t1, RISCV_MAX_ADDR	
	csrw pmpaddr0, t1

	li t1, RISCV_ALL_PERM
	csrw pmpcfg0, t1

	// Set the program counter to nulluser so when we run mret, it runs nulluser
	la t1, nulluser
	csrw mepc, t1

	// Jump to null user
	mret
	.endfunc