/**
 * @file kernel.h 
 *
 * The base include file for the Xinu kernel. Defines symbolic constants,
 * universal return constants, intialization constants, machine size
 * definitions, inline utility functions, and include types
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <stddef.h>

/* Kernel function prototypes */
void nulluser(int, int);

/* Kernel function prototypes */
syscall kputc(uchar);
syscall kgetc(void);

extern void *_end;              /* linker provides end of image       */

#endif                          /* _KERNEL_H_ */