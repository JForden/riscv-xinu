#ifndef  _RISCV_H_
#define  _RISCV_H_

#define RISCV_MSTATUS_MEI_BIT   1<<3 /* IRQs globally disabled on all privilege levels when set to 1. */

#define PREGS   24

#define PREG_A0 0
#define PREG_A1 1
#define PREG_A2 2
#define PREG_A3 3
#define PREG_A4 4
#define PREG_A5 5
#define PREG_A6 6
#define PREG_A7 7
#define PREG_S0 8
#define PREG_S1 9
#define PREG_S2 10
#define PREG_S3 11
#define PREG_S4 12
#define PREG_S5 13
#define PREG_S6 14
#define PREG_S7 15
#define PREG_S8 16
#define PREG_S9 17
#define PREG_S10 18
#define PREG_S11 19
#define PREG_RA 20
#define PREG_SP 21
#define PREG_PC 22

#endif                          /* _RISCV_H_ */