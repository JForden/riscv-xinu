modes 1.1
=========
Xinu uses a three ring structure for process management. Machine, supervisor, and user mode provide adequate methods of interrupt handling and system protection.  The operating system is only in machine mode for a limited time.  

Machine mode:
    Xinu uses machine mode to boot and set system memory bounds before switching to supervisor mode. Before switching to supervisor mode, we adjust the medeleg and mideleg registers. Setting these registers tells the system to delegate all future exceptions and interrupts to the supervisor trap handler. 
Supervisor mode:
    
User mode:
    
