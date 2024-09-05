#include "stack.h"
void canary_tripped(void) {
    printf("AAAJ");
}


unsigned long __stack_chk_guard;
void canary_setup(void)
{
     __stack_chk_guard = 0xBAAAAAAD;//provide some magic numbers
}