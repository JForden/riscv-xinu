#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stddef.h>
#include <stdarg.h>

//I_interupts
//E_exceptions

#define I_RESERVED 0
#define I_SUPER_SOFTWARE 1
#define I_MACHINE_SOFTWARE 3
#define I_SUPERVISOR_TIMER 5
#define I_MACHINE_TIMER 7
#define I_SUPERVISOR_EXTERNAL 9
#define I_MACHINE_EXTERNAL 11
#define E_INSTRUCTION_ADR_MISALIGNED 0
#define E_INSTRUCTION_ACCESS_FAULT 1
#define E_ILLEGAL_INSTRUCTION 2
#define E_BREAKPOINT 3
#define E_LOAD_ADR_MISALIGNED 4
#define E_LOAD_ACCESS_FAULT 5
#define E_STORE_AMO_ADR_MISALIGNED 6
#define E_STORE_AMO_ACCESS_FAULT 7
#define E_ENVCALL_FROM_UMODE 8
#define E_ENVCALL_FROM_SMODE 9
#define E_ENVCALL_FROM_MMODE 11
#define E_INSTRUCTION_PAGEFAULT 12
#define E_LOAD_PAGEFAULT 13
#define E_STORE_AMO_PAGEFAULT 15










void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter);

static inline void
setpc(ulong x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}

#endif                          /* _INTERTUPT_H_ */