#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <stddef.h>
#include <stdarg.h>

void dispatch(ulong cause, ulong val, ulong *frame);

#endif                          /* _INTERTUPT_H_ */