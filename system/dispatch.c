/**
 * @file dispatch.c
 * @provides create, newpid, userret
 *
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */


#include <xinu.h>
#include <interrupt.h>

/**
 * 
 * @ingroup process
 * Dispatch the trap or exception handler, called via interrupt.S
 * @param cause  The value of the scause register 
 * @param stval  The value of the stval register  
 * @param frame  The stack pointer of the process that caused the interupt 
 * @param program_counter  The value of the sepc register 
 */

ulong dispatch(ulong cause, ulong val, ulong program_counter) {
    pcb *proc;
    uint cpuid = gethartid();
    uint pid = currpid[cpuid];

    proc = &proctab[pid];

    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

        if (cause == E_ENVCALL_FROM_UMODE){
            ulong swi_opcode;

            swi_opcode = proc->swaparea[PREG_A7];
			proc->swaparea[PREG_PC] = (ulong)program_counter + 4;
            setpc((ulong)program_counter + 4);
            proc->swaparea[PREG_A0] = syscall_dispatch(swi_opcode, (ulong *)&(proc->swaparea[PREG_A0]));        
        } else {
            xtrap(proc->swaparea, cause, val, program_counter);
        }
    } else {
        // Handle interrupts
        // Remove the MSB.  It should be 1 for interrupts, 0 for exceptions 
        cause = cause << 1;
        cause = cause >> 1;
        //TODO
    }

    kernexit(MAKE_SATP(pid, proc->pagetable));
}