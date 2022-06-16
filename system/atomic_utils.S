#include <riscv.h>

_atomic_compareAndSwapStrong:    
    .func _atomic_compareAndSwapStrong
    lr.w    t0, (a0)
    bne     a1, t0, 1f
    sc.w    t0, a2, (a0)
    bne     t0, zero, _atomic_compareAndSwapStrong
    li      a0, 1
    ret
1:
    mv      a0, zero        // return false
    ret
    .endfunc

.globl _atomic_compareAndSwapWeak
_atomic_compareAndSwapWeak:    
    .func _atomic_compareAndSwapWeak
    lr.w    t0, (a0)
    bne     a1, t0, 1f
    sc.w    t0, a2, (a0)
    bne     t0, zero, 1f
    li      a0, 1
    ret
1:
    mv      a0, zero        // return false
    ret
    .endfunc

.globl _lock_acquire
_lock_acquire:
    .func _lock_acquire
    li t0, 1
again:
    lw t1, (a0) # Check if lock is held.
    bnez t1, again # Retry if held.
    amoswap.w.aq t1, t0, (a0) # Attempt to acquire lock.
    bnez t1, again # Retry if held.
    .endfunc

.globl _lock_release
_lock_release:
    .func _lock_release
    amoswap.w.rl x0, x0, (a0) # Release lock by storing 0.
    .endfunc