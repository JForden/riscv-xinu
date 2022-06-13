#include <xinu.h>

void dispatch(ulong cause, ulong val, ulong *frame) {
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;
        xtrap(frame, cause, val);
    } else {

    }

}