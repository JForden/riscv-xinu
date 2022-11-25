/**
 * @file initialize.c
 * The system begins intializing after the C environment has been
 * established.  After intialization, the null process remains always in
 * a ready or running state.
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/* Function prototypes */
static int sysinit(void);       /* intializes system structures          */
static void welcome(void);      /* Print inital O/S data                 */
extern process main(int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten); /* main is the first process created     */

/* Declarations of major kernel variables */
pcb proctab[NPROC];             /* Process table                         */
qid_typ readylist[NCORES][3];   /* List of READY processes                           */

/* Active system status */
int numproc;                    /* Number of live user processes       */
int currpid[NCORES];            /* Id of currently running proccesses  */

/* Params set by startup.S */
void *memheap;                  /* Bottom of heap (top of O/S stack)     */
ulong cpuid;                    /* Processor id                          */
void *dtb_addr;
ulong *_kernpgtbl;

struct platform platform;       /* Platform specific configuration       */

spinlock_t serial_lock;         /* spinlock for serial port (kprintf)  */

volatile ulong promote_medium[NCORES];
volatile ulong promote_low[NCORES];

/*
 * Intializes the system and becomes the null process.
 * This is where the system begins after the C environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  This routine turns itself into the null process
 * after initialization.  Because the null process must always remain ready
 * to run, it cannot execute code that might cause it to be suspended, wait
 * for a semaphore, or put to sleep, or exit.  In particular, it must not
 * do I/O unless it uses kprintf for synchronous output.
 */
void nullproc(void)
{
    /* null process has nothing else to do but cannot exit  */

    while (1)
    {
        user_yield();
    }
}

void nulluser(void)
{
    int hartid = gethartid();

    if (hartid == 0)
    {
        /* Platform-specific initialization */
        platforminit();

        /* General initialization  */
        sysinit();

        /* Standard Embedded Xinu processor and memory info */
        welcome();

        // Initialize memory protection
        safeInit();

        safeKmapInit();
    }

    /* Call the main program */
    ready(create((void *)nullproc, INITPRIO, "prnull", 0), RESCHED_NO);
    ready(create
          ((void *)main, INITPRIO, "main", 10, 11111, 22222, 33333, 44444,
           55555, 66666, 77777, 88888, 99999, 00000), RESCHED_YES);
}

static void welcome(void)
{
    kprintf(VERSION);
    kprintf("\r\n\r\n");

    /* Output detected platform. */
    kprintf("Detected platform as: [%s] %s (ver %d) RV%dI %s \r\n\r\n",
            platform.manufacturer, platform.family, platform.revision,
            platform.architecture, platform.extensions);

    /* Output Xinu memory layout */
    kprintf("%10d bytes physical memory.\r\n",
            (ulong)platform.maxaddr - (ulong)platform.minaddr);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)platform.minaddr, (ulong)(platform.maxaddr - 1));

    kprintf("%10lu bytes reserved system area.\r\n",
            (ulong)_start - (ulong)platform.minaddr);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)platform.minaddr, (ulong)_start - 1);

    kprintf("%10d bytes Xinu code.\r\n", (ulong)&_end - (ulong)_start);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)_start, (ulong)&_end - 1);

    kprintf("%10d bytes kernel stack space.\r\n",
            (ulong)memheap - (ulong)&_end);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)&_end, (ulong)memheap - 1);
    kprintf("%10d bytes heap space.\r\n",
            (ulong)platform.maxaddr - (ulong)memheap);
    kprintf("           [0x%016lX to 0x%016lX]\r\n\r\n",
            (ulong)memheap, (ulong)platform.maxaddr - 1);

    /*if (PERIPHERALS_BASE < (ulong)platform.maxaddr)
       {
       kprintf("%10d bytes heap space.\r\n",
       (ulong)PERIPHERALS_BASE - (ulong)memheap);
       kprintf("           [0x%08X to 0x%08X]\r\n",
       (ulong)memheap, (ulong)PERIPHERALS_BASE - 1);
       kprintf("%10d bytes memory-mapped peripheral space.\r\n",
       (ulong)platform.maxaddr - (ulong)PERIPHERALS_BASE);
       kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
       (ulong)PERIPHERALS_BASE, (ulong)platform.maxaddr - 1);
       }
       else
       {
       kprintf("%10d bytes heap space.\r\n",
       (ulong)platform.maxaddr - (ulong)memheap);
       kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
       (ulong)memheap, (ulong)platform.maxaddr - 1);
       } */
    kprintf("\r\n");
}

/**
 * Intializes all Xinu data structures and devices.
 * @return OK if everything is initialized successfully
 */
static int sysinit(void)
{
    int i = 0;
    int j = 0;

    /* Initialize process table */
    for (i = 0; i < NPROC; i++)
    {
        proctab[i].state = PRFREE;
    }

    /* initialize null process entry */
    currpid[0] = 0;

    /* Initialize ready lists */
    for (i = 0; i < NCORES; i++)
    {
        for (j = 0; j < 3; j++)
        {
            readylist[i][j] = newqueue();
        }
    }

    for (i = 0; i < NCORES; i++)
    {
        promote_medium[i] = QUANTUM;
        promote_low[i] = QUANTUM;
    }

    for (i = 0; i < NLOCK; i++)
    {
        locktab[i].state = SPINLOCK_FREE;
        locktab[i].lock = SPINLOCK_UNLOCKED;
        locktab[i].core = -1;
    }

    return OK;
}
