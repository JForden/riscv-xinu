#include <riscv.h>

loop:
	.func loop
    call loop
    .endfunc


.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldregs, &newregs, newsatp)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:
	.func ctxsw

	sd ra, PREG_RA*8(a0)
    //sd sp, PREG_SP*8(a0)
    sd s0, PREG_S0*8(a0)
    sd s1, PREG_S1*8(a0)
    sd s2, PREG_S2*8(a0)
    sd s3, PREG_S3*8(a0)
    sd s4, PREG_S4*8(a0)
    sd s5, PREG_S5*8(a0)
    sd s6, PREG_S6*8(a0)
    sd s7, PREG_S7*8(a0)
    sd s8, PREG_S8*8(a0)
    sd s9, PREG_S9*8(a0)
    sd s10, PREG_S10*8(a0)
    sd s11, PREG_S11*8(a0)
    sd ra, PREG_PC*8(a0)

    mv t1, a1
    mv t2, a2

    ld a0, PREG_A0*8(t1)
    ld a1, PREG_A1*8(t1)
    ld a2, PREG_A2*8(t1)
    ld a3, PREG_A3*8(t1)
    ld a4, PREG_A4*8(t1)
    ld a5, PREG_A5*8(t1)
    ld a6, PREG_A6*8(t1)
    ld a7, PREG_A7*8(t1)

    ld ra, PREG_RA*8(t1)
    //ld sp, PREG_SP*8(t1)
    ld s0, PREG_S0*8(t1)
    ld s1, PREG_S1*8(t1)
    ld s2, PREG_S2*8(t1)
    ld s3, PREG_S3*8(t1)
    ld s4, PREG_S4*8(t1)
    ld s5, PREG_S5*8(t1)
    ld s6, PREG_S6*8(t1)
    ld s7, PREG_S7*8(t1)
    ld s8, PREG_S8*8(t1)
    ld s9, PREG_S9*8(t1)
    ld s10, PREG_S10*8(t1)
    ld s11, PREG_S11*8(t1)

    //ld t0, PREG_PC*8(t1)
	
    csrw sscratch, t1

    li t0, RISCV_SPP_TO_U_MODE
    csrc sstatus, t0

    //call ctxsw
    // Load the address of the next program
    ld t0, PREG_PC*8(t1)
	csrw sepc, t0

    sfence.vma zero, zero
    csrw satp, t2
    //call loop
	sfence.vma zero, zero

    sret

    //jalr x0, 0(t0)

	.end ctxsw