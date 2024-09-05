#include "stack.h"
void canary_tripped(void) {
    printf("AAAJ");
}


unsigned long canary_value;
void canary_setup(void)
{
     canary_value = 0xBAAAAAAD;//provide some magic numbers
}