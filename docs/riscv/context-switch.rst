Context Switch
==============

The purpose of context switch in any operating system is to save the current state of registers
so that they can be restored later once the process resumes execution.  Not all registers need to be saved.
The RISC-V calling convention designates which registers are callee-saved (registers that context switch 
function must save) and caller-save (registers that the process must save itself if it desires). 


When saving an outgoing process into memory, the registers must be stored into the corresponding
process control block. The arguments of a process in our implementation of context switch must be
stored in the regs[PREGS] array. When loading an incoming process into memory, our implementation
 of context switch loads the individual arguments into their corresponding argument registers(a0-a7).  
The callee-saved registers are then loaded from their corresponding PCB addresses. With these two 
operations completed, the loaded process should now continue execution from where it last left off.
