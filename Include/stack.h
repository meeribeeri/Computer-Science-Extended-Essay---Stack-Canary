#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void __stack_chk_fail(void);
void __stack_chk_guard_setup(void);
unsigned long __stack_chk_guard;
