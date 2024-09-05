//#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//extern void canary_setup(void);
//extern unsigned long canary_value;
void canary_tripped(void) {
    printf("AAAJ");
}


unsigned long canary_value;
void canary_setup(void)
{
     canary_value = 0xBAAAAAAD;//provide some magic numbers
}

int main() {
    canary_setup();

    unsigned long canary;

    int read_bytes;

    memcpy(&canary, &canary_value, sizeof(canary_value));
    char buf[8];
    printf("canary memory: %p, %p\tbuffer memory: %p\n",canary,canary_value,buf);
    printf("canary address: %p, %p\tbuffer address: %p\n", (void*)&canary,(void*)&canary_value,(void*)&buf);
    //gets(buf);
    scanf("%d", &read_bytes);
    read(0,buf,read_bytes);
printf("canary memory: %p, %p\tbuffer memory: %p\n",canary,canary_value,buf);
    printf("%s",buf);
}