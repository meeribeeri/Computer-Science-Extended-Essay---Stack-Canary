#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void canary_tripped(void);
void canary_setup(void);
unsigned long __stack_chk_guard;
