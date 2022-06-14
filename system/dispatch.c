#include <xinu.h>
#define SOFTWARE 1



void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

        if (cause == 11){
            ulong swi_opcode;
            swi_opcode = frame[PREG_A7];
            setpc((ulong)program_counter + 4);
            frame[PREG_A0] = syscall_dispatch(swi_opcode,(int *)frame[PREG_A0]);        
            return;
        } else {
            xtrap(frame, cause, val);
        }
    } else {
        // Handle interrupts
        // Remove the MSB.  It should be 1 for interrupts, 0 for exceptions 
        cause = cause << 1;
        cause = cause >> 1;
        //TODO
    }

}