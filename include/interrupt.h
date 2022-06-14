#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stddef.h>
#include <stdarg.h>

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter);

static inline void
setpc(ulong x)
{
  asm volatile("csrw mepc, %0" : : "r" (x));
}

#endif                          /* _INTERTUPT_H_ */