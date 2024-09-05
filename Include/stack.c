#include "stack.h"
void __stack_chk_fail(void) {
    printf("AAAJ");
}


unsigned long __stack_chk_guard;
void __stack_chk_guard_setup(void)
{
     __stack_chk_guard = 0xBAAAAAAD;//provide some magic numbers
}