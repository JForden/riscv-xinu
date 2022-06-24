PCB:

    The process control block (PCB) is used in the management of an operating systems’ individual processes. XINU’s PCB is defined in proc.h with a struct called pentry (as shown below).  XINU is limited to the number of processes defined as NPROC at the top of proc.h.  For most platforms, NPROC is 50.  All process information is stored in the global variable proctab.  The process ID can be used as an index in the proctab array to get the PCB for that process.
typedef struct pentry::
    {
        int state;           /**< process state: PRCURR, etc.             */
        int priority;        /**< priority of process                     */
        void *stkbase;       /**< base of run time stack                  */
        int stklen;          /**< stack length                            */
        int core;            /**< core affinity                           */
        uint joinq;          /**< joinq for thread management             */
        char name[PNMLEN];   /**< process name                            */
        ulong regs[PREGS];     /**< stored process registers                */
    } pcb;





Processes can be in a variety of different states that are listed in proc.h.  The states are as follows:
PRFREE (A process is in the PRFREE state when a) the system starts up and no processes have been created or b) after a process is killed)
PRCURR (A process is in the PRCURR state if it is currently running on the CPU)
PRSUSP (Once a process is created by calling create(), it is placed into the PRSUSP state.  It will remain in this state until it is ready to run by calling ready())
PRREADY (Processes in the PRREADY state are marked ready-to-run and will be selected by the CPUs scheduling algorithm to run)





