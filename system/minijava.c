/**
 * @file mj.c
 * For details see http://dx.doi.org/10.1145/1869542.1869568
 */
/* Embedded Xinu, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <device.h>
#include <tty.h>
#include <memory.h>
#include <proc.h>
#include <monitor.h>
#include <yield.h>

syscall _readint(void)
{
    int i = 0, c = 0;

//  control(CONSOLE, TTY_CTRL_SET_IFLAG, TTY_ECHO, 0);
//	control(stdin, TTY_CTRL_CLR_IFLAG, TTY_IRAW, NULL);
//	control(CONSOLE, TTY_CTRL_SET_IFLAG, TTY_ECHO, 0);

//	open(CONSOLE, SERIAL0);

    c = kgetc((int)&devtab[SERIAL0]);//kgetc;
	kputc(c, (int)&devtab[SERIAL0]);
//	c = getchar();
    while (('\n' != c) && ('\r' != c) && (EOF != c))
    {
        if (('0' <= c) && ('9' >= c))
        {
            i = i * 10 + c - '0';
        }

        c = kgetc((int)&devtab[SERIAL0]);
		kputc(c, (int)&devtab[SERIAL0]);
//		c = getchar();
    }
    kprintf("\r\n");
    if (EOF == c)
        return c;

    return i;
}

syscall _printint(int i)
{
    return kprintf("%d\r\n", i);
}

syscall _print(char *s)
{
    return kprintf("%s", s);
}

syscall _println(void)
{
    return kprintf("\r\n");
}

syscall _yield(void)
{
    return yield();
	
}

syscall _sleep(int time)
{
    return sleep(time);
}

int *_new(int n, int init)
{
    int size = (n + 2) * 4;
    int *p = (int *)getmem(size);
    bzero(p, size);
	p[0] = moncreate();
	p[1] = init;
	return p + 2;
}

syscall _lock(int *objAdder)
{
    int *A = objAdder - 2;
    monitor m = (monitor) * A;
    return lock(m);
}

syscall _unlock(int *objAdder)
{
	int *A = objAdder - 2;
    monitor m = (monitor) * A;
    return unlock(m);
}

syscall _threadCreate(int *threadObjAdder)
{
    int *A = threadObjAdder - 1; // back up one word to point to vtable pointer
    int *B = (int *)*A;			 // point to first entry of vtable
    int *C = (int *)*B;			 // point to first method's actual code
    void *procadder = (void *)C;
	static int nextcore = 0;
	nextcore = (nextcore + 1) % 4; 
    return
        ready(create
              (procadder, INITSTK, INITPRIO, "MiniJavaThread", 1,
               threadObjAdder), RESCHED_NO, nextcore);
}


void _BADPTR(void)
{
    kprintf("FATAL ERROR: Null Pointer Exception!\n");
	unsigned int cpuid;
	cpuid = getcpuid();
	kill(currpid[cpuid]);
}

void _BADSUB(void)
{
    kprintf("FATAL ERROR: Index Out Of Bounds Exception!\n");	
	unsigned int cpuid;
	cpuid = getcpuid();
	kill(currpid[cpuid]);
}
