/**
 * @file setupHart.S
 * @provides setupHart
 */
/* Embeddedd Xinu. Copyright (C) 2022. All rights reserved. */

/* gethartid() */
.globl gethartid
gethartid:
	.func gethartid
	mv a0, gp
	ret
	.endfunc

.globl getmisa
getmisa:
	.func getmisa
	csrr a0, misa
	ret
	.endfunc

.globl set_satp
set_satp:
	.func set_satp
	sfence.vma zero, zero
	csrw satp, a0
	sfence.vma zero, zero
	ret
	.endfunc