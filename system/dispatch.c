#include <xinu.h>
#include <interrupt.h>
#define SOFTWARE 1



void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

        if (cause == E_ENVCALL_FROM_SMODE){
            ulong swi_opcode;
            swi_opcode = frame[PREG_A7];
            setpc((ulong)program_counter + 4);
            frame[PREG_A0] = syscall_dispatch(swi_opcode, (ulong *)&frame[PREG_A0]);        
        } else {
            xtrap(frame, cause, val, program_counter);
        }
    } else {
        // Handle interrupts
        // Remove the MSB.  It should be 1 for interrupts, 0 for exceptions 
        cause = cause << 1;
        cause = cause >> 1;
        //TODO
        //TODO2
    }

}